#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>


//Although we commented the code as much as we could read readme in github repo for more info


using namespace std;
// We created a simple struct so we don't have to pass around loose variables
struct Coder {
    int id;
    long long skill;
    int enemy_count;
    double current_priority;
};
int main() {
    // We speed up the standard input/output
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    //Starting the timer of 293 seconds to output best team found after 293 seconds
    auto start_time = chrono::steady_clock::now();
    const double total_time_budget = 293.0; //7 seconds of safety net

    int total_students, total_conflicts;
    if (!(cin >> total_students >> total_conflicts)) return 0;

    vector<long long> skill_levels(total_students + 1);
    for (int i = 1; i <= total_students; ++i) {
        cin >> skill_levels[i];
    }
    vector<vector<int>> enemy_network(total_students + 1);
    vector<int> enemy_counts(total_students + 1, 0);
    for (int i = 0; i < total_conflicts; ++i) {
        int student_a, student_b;
        cin >> student_a >> student_b;
        enemy_network[student_a].push_back(student_b);
        enemy_network[student_b].push_back(student_a);
        enemy_counts[student_a]++;
        enemy_counts[student_b]++;
    }
    vector<int> final_dream_team;
    long long massive_total_score = 0;
    vector<Coder> active_pool;
    
// Step 1: Filtering out the students who get along with everyone immediately and add them to the team. Since M<200000 there will be a lot of such students.
    for (int i = 1; i <= total_students; ++i) {
        if (enemy_counts[i] == 0) {
            final_dream_team.push_back(i);
            massive_total_score += skill_levels[i];
        } else {
            active_pool.push_back({i, skill_levels[i], enemy_counts[i], 0.0});
        }
    }

    // We set up our random number generator to introduce tweaks and jitters.
    mt19937_64 rng(1337);
    //Random tweaks to weightage of skill and degree in the final score of each student: 
    uniform_real_distribution<double> weight_generator(0.1, 10.0);
    // We added some jitter after score calculation to eliminate ties: 
    uniform_real_distribution<double> jitter_generator(0.0, 1e-12);

    long long best_loop_score = -1;
    vector<int> best_loop_roster;

 // Step 2: We keep creating teams sorted based on different weightages towards the skill and degree until time runs out.
    while (true) {
        auto current_time = chrono::steady_clock::now();
        if (chrono::duration<double>(current_time - start_time).count() >= total_time_budget) {
        // We broke the search loop at 293 seconds
            break; 
        }
        // Generate totally new random weights 
        // a-> weightage of the skill level 
	// b-> weightage of the degree
        double random_a = weight_generator(rng);
        double random_b = weight_generator(rng);

        for (auto& person : active_pool) {
            double base_score = (random_a * person.skill) / (random_b * person.enemy_count + 1.0);
            person.current_priority = base_score + jitter_generator(rng);
        }

        // Sort the entire pool of teams based on current_priorirty
        sort(active_pool.begin(), active_pool.end(), [](const Coder& x, const Coder& y) {
            return x.current_priority > y.current_priority;
        });

        vector<bool> in_draft(total_students + 1, false);
        long long draft_score = 0;
        
        //Picking the best available coders without conflicts
        for (const auto& person : active_pool) {
            bool can_join = true;
            for (int enemy : enemy_network[person.id]) {
                if (in_draft[enemy]) {
                    can_join = false;
                    break;
                }
            }
            if (can_join) {
                in_draft[person.id] = true;
                draft_score += person.skill;
            }
        }

        //Trade weaker members already in team for stronger ones still sitting on the bench.
        bool upgraded = true;
        while (upgraded) {
            upgraded = false;
            for (const auto& benched_person : active_pool) {
                if (in_draft[benched_person.id]) continue;

                long long enemy_power_blocking_me = 0;
                vector<int> enemies_to_kick;

                for (int enemy : enemy_network[benched_person.id]) {
                    if (in_draft[enemy]) {
                        enemy_power_blocking_me += skill_levels[enemy];
                        enemies_to_kick.push_back(enemy);
                    }
                }

                // If the benched person is stronger than all blocking enemies combined, swap them (read readme for more info)
                if (benched_person.skill > enemy_power_blocking_me) {
                    for (int enemy_id : enemies_to_kick) {
                        in_draft[enemy_id] = false;
                        draft_score -= skill_levels[enemy_id];
                    }
                    in_draft[benched_person.id] = true;
                    draft_score += benched_person.skill;
                    upgraded = true;
                }
            }
        }
        if (draft_score > best_loop_score) {
            best_loop_score = draft_score;
            best_loop_roster.clear();
            for (const auto& person : active_pool) {
                if (in_draft[person.id]) {
                    best_loop_roster.push_back(person.id);
              }
        }
        }}
        

    // Add our best randomized team to the team of friendly students.
    massive_total_score += best_loop_score;
    for (int id : best_loop_roster) {
        final_dream_team.push_back(id);
    }
    // Sorting and formatting:
    sort(final_dream_team.begin(), final_dream_team.end());

    cout << massive_total_score << "\n";
    for (size_t i = 0; i < final_dream_team.size(); ++i) {
        cout << final_dream_team[i] << (i == final_dream_team.size() - 1 ? "" : " ");
    }
    cout << "\n";

    return 0;
}
