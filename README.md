Our code first identifies students with zero enemies and automatically adds them to the final team. Because the total number of conflicts is relatively low, this filters out tens of thousands of students immediately, massively shrinking our processing pool and speeding up the program.

The remaining students enter a continuous search loop that runs for exactly 293 seconds, leaving a 7 second safety net to ensure safe execution. In each loop, every student gets a priority score by dividing their skill by their enemy count. To explore different team combinations, we multiply the skill and enemy counts by completely random numbers each time, allowing the algorithm to smoothly shift between prioritizing raw skill or friendly students. We also add a microscopic random jitter to break ties.

For each coder the score is calculated as: 

Total priority score = (a* Skill level)/ (b*number of conflicts this coder has + 1).

Here the coders with more skill are preferred but the coders with more conflicts are penalized too!
Where a and b are random numbers between 0.1 to 10. 
When a and b are almost same this denotes equal weightages. a>>b would create a team with more weightage to skills and b>>a would make a team where coders with more conflicts are punished more.

After sorting students by priority, we run a fast draft, adding people to the team if their enemies are not already inside. Then we perform strategic bench swaps. The code checks if a benched student (a student not chosen) is mathematically stronger than the combined power of their enemies currently on the team. If so, it kicks the weaker enemies off and swaps the stronger student in. This upgrade cycle repeats until no further score improvements can be made.

The program constantly tracks the highest scoring team found during the 5 minute window. When time runs out, it combines this best drafted team with the friendly students saved at the start, sorts their IDs in ascending order, and prints the final maximum score and roster.
