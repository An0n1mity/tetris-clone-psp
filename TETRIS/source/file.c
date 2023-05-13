#include <string.h>    
#include <stdio.h>

int isScoreTopScore(const unsigned int score)
{
    FILE* file = fopen("scores/top_score.txt", "r+");
    if(file)
    {
        char line[100];
        while(fgets(line, 100, file))
        {
            if(strlen(line) > 0)
            {
                line[strlen(line)-1] = '\0';
                const char* player_score_in_file = strtok(line, " ");
                player_score_in_file = strtok(NULL, " ");
                if(score > atoi(player_score_in_file))
                {
                    return 1;
                }
            }
        }
    }
    fclose(file);
    return 0;
}

void changeTopScore(unsigned int score, unsigned int level, unsigned int place)
{
    FILE* file = fopen("scores/top_score.txt", "r+");
    FILE* tmp_file = fopen("scores/tmp.txt", "w");
    char new_line[100];
    size_t idx = 0;
    if(file)
    {
        char line[100];
        while(fgets(line, 100, file))
        {
            //line[strlen(line)-1] = '\0';
            if(place == idx)
            {
                const char* player_score_in_file = strtok(line, " ");
                sprintf(new_line, "%s %d %d", player_score_in_file, score, level);
                fprintf(tmp_file, new_line);
            }
            else
                fprintf(tmp_file, line);

            idx++;
        }
    }
    fclose(file);
    remove("scores/top_score.txt");
    rename("scores/tmp.txt", "scores/top_score.txt");
    fclose(tmp_file);
}

void changePlayerName(unsigned int place, const char* new_name)
{
    FILE* file = fopen("scores/top_score.txt", "r+");
    FILE* tmp_file = fopen("scores/tmp.txt", "w");
    char new_line[100];
    size_t idx = 0;
    if(file)
    {
        char line[100];
        while(fgets(line, 100, file))
        {
            //line[strlen(line)-1] = '\0';
            if(place == idx)
            {
                const char* player_score_in_file = strtok(line, " ");
                player_score_in_file = strtok(NULL, " ");
                const char* score = player_score_in_file;
                player_score_in_file = strtok(NULL, " ");
                const char* level = player_score_in_file;

                sprintf(new_line, "%s %s %s", new_name, score, level);
                fprintf(tmp_file, new_line);
            }
            else
                fprintf(tmp_file, line);

            idx++;
        }
    }
    fclose(file);
    remove("scores/top_score.txt");
    rename("scores/tmp.txt", "scores/top_score.txt");
    fclose(tmp_file);
}

char** getPlayersNames()
{
    FILE* file = fopen("scores/top_score.txt", "r+");
    char** players_names = malloc(sizeof(char*)*3);
    size_t idx = 0;
    if(file)
    {
        char line[100];
        while(fgets(line, 100, file))
        {
            if(strlen(line) > 0)
            {
                line[strlen(line)-1] = '\0';
                const char* player_score_in_file = strtok(line, " ");
                players_names[idx] = strdup(player_score_in_file);
                idx++;
            }
        }
    }
    fclose(file);
    return players_names;
}

char** getPlayersLevels()
{
    FILE* file = fopen("scores/top_score.txt", "r+");
    char** players_levels = calloc(3, sizeof(char*));
    for (size_t i = 0; i < 3; i++)
    {
        players_levels[i] = malloc(sizeof(char) * 3);
        for (size_t j = 0; j < 2; j++)
        {
            players_levels[i][j] = '0';
        }
        
    }
    
    size_t idx = 0;
    if(file)
    {
        char line[100];
        while(fgets(line, 100, file))
        {
            if(strlen(line) > 0)
            {
                line[strlen(line)-1] = '\0';
                const char* player_level_in_file = strtok(line, " ");
                player_level_in_file = strtok(NULL,  " ");
                printf("%s\n", player_level_in_file);

                player_level_in_file = strtok(NULL, " ");
                printf("%s\n", player_level_in_file);
                strcpy(players_levels[idx], player_level_in_file);
                idx++;
            }
        }
    }
    fclose(file);

    return players_levels;
}

main(){
    int score = 500;
    const char* player_name = "TEST";

    if(isScoreTopScore(400))
        printf("400 is a top score\n");
    char** players_names = getPlayersNames();
    printf("changing score\n");
    changeTopScore(1000, 5, 1);
    changePlayerName(0, "ALEXA");
}