#include "minimizer.h"
#include <algorithm>
#include <unordered_map>
#include <climits>



struct Implicant {
    int bits;   // fixed bits (where mask has 0)
    int mask;   // 1 means "don't care" in that bit position
    bool used;

    bool covers(int m) const {
        int fixedMask = ~mask;
        return (m & fixedMask) == (bits & fixedMask);
    }
};

static int popcountInt(int x) {
    return __builtin_popcount((unsigned)x);
}

static std::vector<Implicant> uniqueImplicants(const std::vector<Implicant>& v) {
    std::vector<Implicant> out = v;
    std::sort(out.begin(), out.end(), [](const Implicant& a, const Implicant& b) {
        if (a.mask != b.mask) return a.mask < b.mask;
        return a.bits < b.bits;
    });

    out.erase(std::unique(out.begin(), out.end(), [](const Implicant& a, const Implicant& b) {
        return a.bits == b.bits && a.mask == b.mask;
    }), out.end());

    return out;
}

// combine if same mask and differ in exactly one FIXED bit
static bool combineIfPossible(const Implicant& a, const Implicant& b, Implicant& out) {
    if (a.mask != b.mask) return false;

    int diff = a.bits ^ b.bits;

    // must differ in exactly one bit
    if ((diff & (diff - 1)) != 0) return false;

    // that differing bit cannot already be a don't-care
    if (diff & a.mask) return false;

    out.mask = a.mask | diff;   
    out.bits = a.bits & ~diff; 
    out.used = false;
    return true;
}

static std::string implicantToTerm(const Implicant& imp, int n) {
    static const char vars[4] = {'A','B','C','D'};
    std::string term;

    for (int i = 0; i < n; i++) {
        int bitPos = (n - 1 - i); 
        int bitMask = 1 << bitPos;

        if (imp.mask & bitMask) {
            // variable eliminated in this group
            continue;
        }

        bool isOne = (imp.bits & bitMask) != 0;
        term.push_back(vars[i]);
        if (!isOne) term.push_back('\''); 
    }

    if (term.empty()) return "1";
    return term;
}

MinimizeResult minimize(int n, const std::vector<int>& minterms) {
    MinimizeResult result;
    int total = 1 << n;

    // Edge cases
    if (minterms.empty()) {
        result.terms.push_back("0");
        return result;
    }
    if ((int)minterms.size() == total) {
        result.terms.push_back("1");
        GroupInfo g;
        g.groupSize = total;
        g.minterms = minterms;
        g.term = "1";
        result.groups.push_back(g);
        return result;
    }

    // 1) Start implicants from each minterm (each single 1 on the K-map)
    std::vector<Implicant> current;
    for (int m : minterms) {
        current.push_back({m, 0, false});
    }
    current = uniqueImplicants(current);

    // 2) Repeatedly combine (this is like making bigger and bigger K-map groups)
    std::vector<Implicant> primes;

    while (!current.empty()) {
        for (auto& imp : current) imp.used = false;

        std::vector<Implicant> nextRound;

        for (int i = 0; i < (int)current.size(); i++) {
            for (int j = i + 1; j < (int)current.size(); j++) {
                Implicant combined;
                if (combineIfPossible(current[i], current[j], combined)) {
                    current[i].used = true;
                    current[j].used = true;
                    nextRound.push_back(combined);
                }
            }
        }

        // Any implicant not used in combining is a PRIME implicant (a “maximal group”)
        for (const auto& imp : current) {
            if (!imp.used) primes.push_back(imp);
        }

        current = uniqueImplicants(nextRound);
    }

    primes = uniqueImplicants(primes);

    // 3) Build coverage info (which prime covers which minterms)
    std::vector<std::vector<int>> coversList(primes.size());
    std::unordered_map<int, std::vector<int>> coveredBy;

    for (int p = 0; p < (int)primes.size(); p++) {
        for (int m : minterms) {
            if (primes[p].covers(m)) {
                coversList[p].push_back(m);
                coveredBy[m].push_back(p);
            }
        }
        std::sort(coversList[p].begin(), coversList[p].end());
    }

    // 4) Essential prime implicants (minterms covered by only one prime)
    std::vector<bool> chosen(primes.size(), false);
    std::vector<bool> covered(total, false);

    for (int m : minterms) {
        if ((int)coveredBy[m].size() == 1) {
            chosen[coveredBy[m][0]] = true;
        }
    }

    for (int p = 0; p < (int)primes.size(); p++) {
        if (!chosen[p]) continue;
        for (int m : coversList[p]) covered[m] = true;
    }

    // 5) Cover remaining minterms with a minimal extra set (small brute force; OK for <=4 vars)
    std::vector<int> remaining;
    for (int m : minterms) {
        if (!covered[m]) remaining.push_back(m);
    }

    if (!remaining.empty()) {
        std::vector<int> candidates;
        for (int p = 0; p < (int)primes.size(); p++) {
            if (chosen[p]) continue;
            bool useful = false;
            for (int m : remaining) {
                if (primes[p].covers(m)) { useful = true; break; }
            }
            if (useful) candidates.push_back(p);
        }

        int c = (int)candidates.size();
        int bestMask = -1;
        int bestCount = INT_MAX;

        for (int mask = 1; mask < (1 << c); mask++) {
            int count = popcountInt(mask);
            if (count >= bestCount) continue;

            bool ok = true;
            for (int m : remaining) {
                bool isCovered = false;
                for (int i = 0; i < c; i++) {
                    if (mask & (1 << i)) {
                        int p = candidates[i];
                        if (primes[p].covers(m)) { isCovered = true; break; }
                    }
                }
                if (!isCovered) { ok = false; break; }
            }

            if (ok) {
                bestCount = count;
                bestMask = mask;
            }
        }

        if (bestMask != -1) {
            for (int i = 0; i < c; i++) {
                if (bestMask & (1 << i)) {
                    chosen[candidates[i]] = true;
                }
            }
        }
    }

    // 6) Build final terms + groups explanation
    std::vector<std::string> terms;
    std::vector<GroupInfo> groups;

    for (int p = 0; p < (int)primes.size(); p++) {
        if (!chosen[p]) continue;

        std::string term = implicantToTerm(primes[p], n);
        terms.push_back(term);

        GroupInfo g;
        int eliminated = popcountInt(primes[p].mask);
        g.groupSize = 1 << eliminated;
        g.minterms = coversList[p];
        g.term = term;
        groups.push_back(g);
    }

    // remove duplicate terms (rare but safe)
    std::sort(terms.begin(), terms.end());
    terms.erase(std::unique(terms.begin(), terms.end()), terms.end());

    result.terms = terms;
    result.groups = groups;
    return result;
}
