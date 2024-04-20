#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
} candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Assignment includes everything below

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    // TODO
    // Look through candidates to find the one with provided name
    for (int i = 0; i < candidate_count; i++)
    {
        // Compare candidates name with the provided name
        if (strcmp(name, candidates[i].name) == 0)
        {
            // If name matches, update preferences array
            preferences[voter][rank] = i;
            return true;
        }
    }
    // If the candidate with the given name is not found, the vote is invalid.
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    /* TODO: update number of votes each candidate
        has at this stage in the runoff*/

    // Iterate through each voter
    for (int i = 0; i < voter_count; i++)
    {
        // Iterate through each voter's preference
        for (int j = 0; j < candidate_count; j++)
        {
            int preferred_candidate = preferences[i][j];

            // Check if the preferred candidate is eliminated
            if (!candidates[preferred_candidate].eliminated)
            {
                // Update votes for preferred candidate
                candidates[preferred_candidate].votes++;
                break;
            }
        }
    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    // TODO
    // Calculate the number of votes needed to win the election
    int votes_needed_to_win = (voter_count / 2) + 1;

    // Iterate through candidates to checl if any candidate has enough votes to win
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes >= votes_needed_to_win)
        {
            printf("%s\n", candidates[i].name);
            // If candidate has enough to win return true
            return true;
        }
    }
    // If no candidate has enough to win, return false
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    // TODO
    // Initialize min_votes with large value
    int min_votes = MAX_VOTERS;

    // Iterate through cadnidates to find the minimum vote among non-eliminated candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // Check if the candidate is still in the election and has fewer votes than current minimum
        if (!candidates[i].eliminated && candidates[i].votes < min_votes)
        {
            // Updates min_votes if a candidate has fewer votes
            min_votes = candidates[i].votes;
        }
    }
    // Return minimum vote count
    return min_votes;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    // TODO
    // Iterate through candidates to check if every non-elimionated candiate has the same number of
    // votes
    for (int i = 0; i < candidate_count; i++)
    {
        // Check if the candidate is still in the election and if their votes are different from the
        // minimum
        if (!candidates[i].eliminated && candidates[i].votes != min)
        {
            // If any candidate has a different number of votes, its not a tie
            return false;
        }
    }
    // if all non-eliminated candidate have the same number of votes as the minimum, its a tie
    return true;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    // TODO
    // Iterate through candidates to eliminate candidates with the minimum number of votes
    for (int i = 0; i < candidate_count; i++)
    {
        // Check if the candidate has the minimum number of votes and is still in the elction
        if (candidates[i].votes == min && !candidates[i].eliminated)
        {
            // Eliminate candidates
            candidates[i].eliminated = true;
        }
    }
    return;
}
