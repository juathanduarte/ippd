#include <iostream>
#include <vector>
#include <omp.h>
#include <cstdlib>
#include <iomanip>

using namespace std;

void insertionSort(vector<int>& bucket) {
    int n = bucket.size();
    for (int i = 1; i < n; i++) {
        int key = bucket[i];
        int j = i - 1;
        while (j >= 0 && bucket[j] > key) {
            bucket[j + 1] = bucket[j];
            j = j - 1;
        }
        bucket[j + 1] = key;
    }
}

void parallelBucketSort(vector<int>& arr, int num_buckets) {
    vector<vector<int>> buckets(num_buckets);

    int bucket_range = 1000 / num_buckets;

    #pragma omp parallel for
    for (int i = 0; i < arr.size(); i++) {
        int bucket_idx = min(arr[i] / bucket_range, num_buckets - 1);
        #pragma omp critical
        buckets[bucket_idx].push_back(arr[i]);
    }

    arr.clear();
    cout << "\n--- Vetor ordenado (por baldes):" << endl;
    for (int i = 0; i < num_buckets; i++) {
        insertionSort(buckets[i]);

        int min_limit = i * bucket_range;
        int max_limit = (i + 1) * bucket_range - 1;

        cout << "- Balde " << i + 1 << " (Limites " << min_limit << " - " << max_limit << "):" << endl;

        for (int j = 0; j < buckets[i].size(); j++) {
            cout << buckets[i][j] << " ";
            arr.push_back(buckets[i][j]);
        }
        cout << endl << endl;
    }
}

int getTerminalWidth() {
    const char* cmd = "tput cols";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) {
        cerr << "Erro ao chamar tput para obter a largura do terminal." << endl;
        return 80;
    }

    char buffer[128];
    fgets(buffer, sizeof(buffer), pipe);
    pclose(pipe);

    return atoi(buffer);
}

void printCenteredTitle(const string& title) {
    int terminal_width = getTerminalWidth();
    int padding = (terminal_width - title.size()) / 2;
    cout << setw(padding + title.size()) << title << endl;
}

int main() {
    const int size = 1000;
    const int num_buckets = 10;

    string title = "Bucket sort - Introdução a processamento paralelo e distribuído";
    printCenteredTitle(title);

    vector<int> arr(size);
    srand(time(NULL));
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 1001;
    }

    cout << "--- Vetor desordenado:" << endl;
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " | ";
    }
    cout << endl;

    parallelBucketSort(arr, num_buckets);

    cout << "--- Vetor ordenado:" << endl;
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " | ";
    }

    return 0;
}
