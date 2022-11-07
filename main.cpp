#include <bits/stdc++.h>
using namespace std;

#define POPULATION_SIZE 100
#define ITERATIONS 10000

bool get_random_boolean(int percentage_of_true){
    return (1 + (rand() % 101)) < percentage_of_true;
}

int get_random_integer(int lower_range, int upper_range){
    return (lower_range + (rand() % (upper_range - lower_range + 1)));
}

long long fitness(const vector <bool>& individual, const vector<pair<int, int>>& items){
    long long result = 0;
    for (int i = 0; i < individual.size(); i++){
        if (!individual[i]) continue;
        result += items[i].second;
    }
    return result;
}

void print_individual(const pair<long long, vector <bool>> &individual, const vector<pair<int, int>> &items){
    cout << "------------------------------------------\n";
    cout << "Best Fitness Acquired -> " << individual.first << '\n';
    int total_weight = 0, total_value = 0;
    cout << "Items in the bag\n";
    cout << "Indices : ";
    for (int i = 0; i < individual.second.size(); i++){
        if (individual.second[i]){
            total_weight += items[i].first;
            total_value += items[i].second;
            cout << i + 1 << " ";
        }
    }
    cout << "\nTotal weight : " << total_weight;
    cout << "\nTotal value : " << total_value;
    cout << "\n";
    cout << "------------------------------------------\n";
}

void doWork(){
    //Input
    long long bag_size;
    cin >> bag_size;
    int number_of_items;
    cin >> number_of_items;
    //first -> weight, second -> value
    vector <pair<int, int>> items(number_of_items);
    for (auto &i : items) cin >> i.first >> i.second;

    //Random Engine for generating random booleans
    srand((int) time(nullptr));

    // first -> fitness, second -> individual's chromosomes
    vector< pair<long long, vector<bool>> > population(
            POPULATION_SIZE,
            {0, vector<bool>(number_of_items, false)}
        );

    //Random Population
    for (int i = 0; i < POPULATION_SIZE; i++){
        long long current_weight = 0;

        for (int j = 0; j < number_of_items; j++){
            if (!get_random_boolean(50)) continue;
            if (current_weight + items[j].first > bag_size) break;
            population[i].second[j] = true;
            current_weight += items[j].first;
        }

        population[i].first = fitness(population[i].second, items);

    }
    auto *best_individual = new pair<long long, vector<bool>>(0, vector<bool>(number_of_items, false));
    for (int i = 0; i < ITERATIONS; i++){
        sort(population.begin(), population.end());
        if (population[POPULATION_SIZE - 1].first > best_individual->first){
            best_individual = &population[POPULATION_SIZE - 1];
        }
        vector <int> rank(POPULATION_SIZE);
        rank[0] = 1;
        for (int j = 1; j < POPULATION_SIZE; j++){
            rank[j] = rank[j-1] + (j + 1);
        }


        long long first_parent_index = upper_bound(
                rank.begin(), rank.end(),
                get_random_integer(1, (POPULATION_SIZE * (POPULATION_SIZE + 1) / 2) - 1)
                ) - rank.begin();

       long long second_parent_index = upper_bound(
               rank.begin(), rank.end(),
               get_random_integer(1, (POPULATION_SIZE * (POPULATION_SIZE + 1) / 2) - 1)
       ) - rank.begin();

       int crossover_index = get_random_integer(0, number_of_items - 1);

       pair<long long, vector <bool>> first_child(0, vector<bool>(number_of_items, false));

       int first_child_weight = 0;
       for (int j = 0; j < crossover_index; j++) {
           if ( population[first_parent_index].second[j]) first_child_weight += items[j].first;
           if (first_child_weight > bag_size) break;
           first_child.second[j] = population[first_parent_index].second[j];
       }
       for (int j = crossover_index; j < number_of_items; j++) {
           if (population[second_parent_index].second[j]) first_child_weight += items[j].first;
           if (first_child_weight > bag_size) break;
           first_child.second[j] = population[second_parent_index].second[j];
       }

       pair<long long, vector <bool>> second_child(0, vector<bool>(number_of_items, false));

       int second_child_weight = 0;
       for (int j = 0; j < crossover_index; j++) {
           if ( population[second_parent_index].second[j]) second_child_weight += items[j].first;
           if (second_child_weight > bag_size) break;
           second_child.second[j] = population[second_parent_index].second[j];
       }
       for (int j = crossover_index; j < number_of_items; j++) {
           if (population[first_parent_index].second[j]) second_child_weight += items[j].first;
           if (second_child_weight > bag_size) break;
           second_child.second[j] = population[first_parent_index].second[j];
       }


       population[0] = first_child;
       population[1] = second_child;

       //Mutation
       if (get_random_boolean(10)){
           int index_of_mutation = get_random_integer(0, number_of_items - 1);
           if (population[0].second[index_of_mutation] || first_child_weight + items[index_of_mutation].first <= bag_size){
                population[0].second[index_of_mutation] = !population[0].second[index_of_mutation];
           }
       }

       if (get_random_boolean(10)){
           int index_of_mutation = get_random_integer(0, number_of_items - 1);
           if (population[1].second[index_of_mutation] || second_child_weight + items[index_of_mutation].first <= bag_size){
               population[1].second[index_of_mutation] = !population[1].second[index_of_mutation];
           }
       }

       population[0].first = fitness(first_child.second, items);
       population[1].first = fitness(second_child.second, items);

    }

    print_individual(*best_individual, items);
}


int main() {
    //From file to std cin/cout
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    //Main Function
    int test_cases;
    cin >> test_cases;
    while (test_cases--){
        cout << test_cases + 1 << '\n';
        doWork();
    }
    return 0;
}
