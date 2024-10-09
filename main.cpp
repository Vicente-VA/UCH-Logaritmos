#include <iostream>
#include <cstdint>
#include <random>
#include <array>

#define PAGE_SIZE 1024
#define ELEMENT_SIZE 64

struct Page {
    std::array<long long, 16> items;
    long long n_items;

    Page() : n_items(0), items({}) { } // Constructor to initialize n_items
};

class HashingTable {
private:
    long long p; // Cantidad de celdas de la tabla
    long long t; // Nivel actual
    int max_cost;

    long long randomULL() {
        std::random_device rd;
        std::mt19937_64 gen(rd());
        std::uniform_int_distribution<long long> dist(1, 1 << 16); // UINT64_MAX es 2^64 - 1
        return dist(gen);
    }

    long long a;
    long long b;

    long long hashingFunc(long long y) {
        return (a * y + b);
    }

public:
    std::vector<std::vector<Page> > table; // Table
    HashingTable(int max_cost) : p(1), t(0), max_cost(max_cost), a(randomULL()), b(randomULL()) {
        table.emplace_back(std::vector<Page>{Page()}); // First page
        table.emplace_back(std::vector<Page>{Page()}); // Second page
    }

    void insert(long long y) {
        long long k = hashingFunc(y) % (1 << (t + 1)); // k = h(y) mod 2^(t+1)
        k = (p <= k) ? k - (1 << t) : k; // if p <= k the page is not yet created

        bool was_inserted = false;
        for (auto &page: table[k]) {
            if (page.n_items < 16) {
                page.items[page.n_items - 1] = y;
                page.n_items++;
                was_inserted = true;
                break;
            }
        }
        if (!was_inserted){
            Page p = Page();
            p.items[0] = y;
            table[k].emplace_back(p);
        }

        long long total_rebalses = 0;
        long long con_rebalse = 0;

        for (auto &page_vector: table) {
            if (page_vector.size() > 1) {
                con_rebalse++;
                total_rebalses += page_vector.size() - 1;
            }
        }

        double accesos_promedio = total_rebalses / con_rebalse;

        if (accesos_promedio > max_cost) {
            expand_table();
        }
    };


    void expand_table() {
        auto &sig = table[p - static_cast<long long>(pow(2, t))];
        std::vector<Page> nuevo_p = {}; // New page for redistribution

        // Increment the number of pages
        p++;

        // Redistribute items from the overflow page (sig)
        for (auto &page: sig) {
            for (auto &item: page.items) {
                long long new_k = hashingFunc(item);

                if (new_k < p - static_cast<long long>(pow(2, t))) {
                    // Item belongs in the original page (sig)
                    sig[0].items.push_back(item);
                    sig[0].n_items++;
                } else {
                    // Item belongs in the new page (nuevo_p)
                    if (nuevo_p.empty() || nuevo_p.back().n_items >= 16) {
                        nuevo_p.emplace_back();
                    }
                    nuevo_p.back().items.push_back(item);
                    nuevo_p.back().n_items++;
                }
            }
        }

        // Remove all overflow pages in 'sig' except the compacted one
        sig.resize(1);

        // Add the new page to the table
        table.emplace_back(nuevo_p);

        // Update 't' if needed
        if (p == (1LL << (t + 1))) {
            t++;
        }
    }

    // void expand_table() {
    //     auto sig = table[p - static_cast<long long>(pow(2,t))];
    //     std::vector<Page> nuevo_p =  {};
    //
    //     p++;
    //
    //     for (auto &page: sig) {
    //
    //     }
    // }
};


long long* genNnumbers(long long N) {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<unsigned long long> dist(
            std::numeric_limits<std::int64_t>::min(),
            std::numeric_limits<std::int64_t>::max()
    );
    long long* numbers = new long long[N];
    for (long long i = 0; i < N; i++) {
        numbers[i] = dist(gen);
    }
    return numbers;
}


#define ELEMENTS_TO_INSERT 9000
#define MAX_COST_ALLOWED 10.0

int main() {
    HashingTable ht(MAX_COST_ALLOWED);

    // Insert some elements and force expansion
    for (long long i = 1; i <= ELEMENTS_TO_INSERT; i++) {
        ht.insert(i);
    }

    // Print out the content of the hash table
    std::cout << "Hash Table Content:" << std::endl;
    for (size_t i = 0; i < ht.table.size(); ++i) {
        std::cout << "Bucket " << i << ": ";
        for (auto &page : ht.table[i]) {
            std::cout << "[ ";
            for (auto &item : page.items) {
                std::cout << item << " ";
            }
            std::cout << "] ";
        }
        std::cout << std::endl;
    }

    return 0;
}
