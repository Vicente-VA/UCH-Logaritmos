#include <iostream>
#include <cstdint>
#include <random>

#define PAGE_SIZE 1024;
#define ELEMENT_SIZE 64;

struct HT_item {
    long long key;
    long long value;
};

struct Page {
    HT_item items[16];
    int n_items;
};

struct HT {
    Page** pages;
    int n_pages;
    int t;
    int p;

    HT* HT_init() {
        HT* table = new HT;
        table->pages = new Page*[1]; // Inicialmente 1 página
        table->pages[0] = new Page;
        table->pages[0]->n_items = 0;
        table->n_pages = 1;
        table->t = 0;
        table->p = 0;
        return table;
    }

};

uint64_t HT_function() {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dist(0, UINT64_MAX); // UINT64_MAX es 2^64 - 1
    return dist(gen);
}

void HT_insert(HT* table, long long key, long long value) {
    uint64_t hash_value = HT_function();
    int k = hash_value % (1 << (table->t + 1));

    if (k < table->p) {
        // Insertar en la página k
        if (table->pages[k]->n_items < 16) {
            table->pages[k]->items[table->pages[k]->n_items++] = {key, value};
        } else {
            // Manejar rebalse (crear nueva página o expansión)
            // ...
        }
    } else {
        // Insertar en la página k - 2^t
        // Similar al caso anterior, pero con expansión
        // ...
    }

    // Verificar si se debe expandir la página p
    // ...
}

void HT_expand(HT* table) {
    int old_p = table->p;
    int new_p = table->p + 1;
    table->pages = (Page**) realloc(table->pages, sizeof(Page*) * (table->page_count + 1));
    table->pages[new_p] = new Page;
    table->pages[new_p]->n_items = 0;
    table->n_pages++;

    // Reinsertar los elementos de la página p - 2^t
    // ...

    table->p++;
    if (table->p == (1 << (table->t + 1))) {
        table->t++;
    }
}


template <typename E>
void blah(E& engine) {
    std::uniform_int_distribution<unsigned long long> dis(
            std::numeric_limits<std::int64_t>::min(),
            std::numeric_limits<std::int64_t>::max()
    );
    needs_random(dis(engine));
}

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


int main() {
    return 0;
}
