// Code by Samuel Jamieson for Operating Systems 1 - CS374
// ONID: 934325118

//Libraries for basic code functions
#include <stdio.h>        // Input and Output (I/O) Library for reading keyboard Inputs and using basic C functions
#include <stdlib.h>       // Functions for memory allocation and process control
#include <string.h>       // Manipulation for standard C strings
char *strdup(const char *s);

// Define a struct for movie
typedef struct movie {
    char *title;          // Movie Title from CSV
    int year;             // Release year from CSV
    char *languages;      // Languages in which movie was released
    double rating;        // Average rating of movie
    struct movie *next;   // Pointer to the next movie in the Linked List
} Movie;

// Function declarations (optional but good practice, Yes its annoying but I should do it in case I forget)
Movie* create_movie(char *title, int year, char *languages, double rating);
void add_movie(Movie **head, char *title, int year, char *languages, double rating);
void print_movies(Movie *head);
void free_movies(Movie *head);
int process_file(const char *filename, Movie **head);
void show_movies_by_year(Movie *head, int year);
void show_highest_rated_by_year(Movie *head);
void show_movies_by_language(Movie *head, const char *language);
void display_menu(Movie *head);

// Function definitions (For the functions declared above just now)
Movie* create_movie(char *title, int year, char *languages, double rating) {
    Movie *new_movie = (Movie*)malloc(sizeof(Movie));    // Allocating Memory
    new_movie->title = strdup(title);                    
    new_movie->year = year;
    new_movie->languages = strdup(languages);
    new_movie->rating = rating;
    new_movie->next = NULL;                              // Initailizing the next pointer to be NULL (prepping for next CSV read in)
    return new_movie;                                    // Return the newly read in movie
}

// Adds a new movie to the end of the linked list from CSV
void add_movie(Movie **head, char *title, int year, char *languages, double rating) {
    Movie *new_movie = create_movie(title, year, languages, rating);
    if (*head == NULL) {
        *head = new_movie;
    } else {                // Traverse to the end of the linked list and append the new movie
        Movie *temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_movie;
    }
}

// Frees allocated memory for the linked list of movies
void free_movies(Movie *head) {
    Movie *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp->title);        // Free the dynamically allocated title
        free(temp->languages);    // Frees the dynamically allocated languages
        free(temp);               // Frees the movie struct itself
    }
}

// Processes the CSV file into inputs usable by the computer and populates the linked list with movie data
int process_file(const char *filename, Movie **head) {
    FILE *file = fopen(filename, "r");                  // Open the file for reading
    if (file == NULL) {                                 // Check if the file was opened successfully (Should add print statement saying it opened for debug purposes)
        printf("Could not open file %s\n", filename);
        return -1;
    }

    char line[1024];                                    // Buffer to hold each line from the CSV file
    int movie_count = 0;
    fgets(line, sizeof(line), file);                    // Skip the header line

    // Reads the rest of the file Line by Line (this feels inefficient to me) (email me if there is a more effective way I can learn about?)
    while (fgets(line, sizeof(line), file)) {
        char *title = strtok(line, ",");
        int year = atoi(strtok(NULL, ","));        // Convert Year input to a string
        char *languages = strtok(NULL, ",");
        double rating = atof(strtok(NULL, ","));   // Convert Rating from a string to a double

        // Added the Parsed Movie data to the Linked list (Should I add this in order? It doesnt say so but it would look nice!)
        add_movie(head, title, year, languages, rating);
        movie_count++;
    }

    fclose(file);                                                                            // Close file after reading in data
    printf("Processed file %s and parsed data for %d movies\n", filename, movie_count);      // Print message about how many movies were populated
    return movie_count;
}

// Displays all movies released in specified year
void show_movies_by_year(Movie *head, int year) {
    Movie *temp = head;
    int found = 0;                          // Flag to track if any movies were found
    while (temp != NULL) {                  // While loop traverses linked list to check release date of each movie
        if (temp->year == year) {
            printf("%s\n", temp->title);    // Print title of movie if it matchs year specified
            found = 1;
        }
        temp = temp->next;
    }
    
    // If no movies were found print this
    if (!found) {
        printf("No data about movies released in the year :( %d\n", year);
    }
}

// Shows highest rated movie for each year
void show_highest_rated_by_year(Movie *head) {
    Movie *temp = head;
    // Array to store highest ratings and best movies for each year
    double highest_ratings[2022] = {0};    // Initalize all ratings to 0 (initialize the array)
    Movie *best_movies[2022] = {NULL};     // Initialize all movie pointers to 0

    // Traverse the Linked list to update the best movies for each year
    while (temp != NULL) {
        int year = temp->year;
        if (temp->rating > highest_ratings[year]) {
            highest_ratings[year] = temp->rating;
            best_movies[year] = temp;
        }
        temp = temp->next;
    }

    // Print the highest rated movie for each year that exists in data
    for (int i = 1900; i <= 2021; i++) {
        if (best_movies[i] != NULL) {
            printf("%d %.1f %s\n", i, highest_ratings[i], best_movies[i]->title);
        }
    }
}

// Display movie by language specified
void show_movies_by_language(Movie *head, const char *language) {
    Movie *temp = head;
    int found = 0;                                          // Flag to track if any movies were found
    // Traverse the linked list and check language field of each moive                                        
    while (temp != NULL) {
        if (strstr(temp->languages, language) != NULL) {    // Check is language is in the string
            printf("%d %s\n", temp->year, temp->title);     // Print the year and title of the matching movie
            found = 1;
        }
        temp = temp->next;
    }
    // If no movies were found print this
    if (!found) {
        printf("No data about movies released in %s\n. What a boring year!", language);
    }
}

// Displays menu choices for User (I love making menus :) )
void display_menu(Movie *head) {
    int choice;
    while (1) {
        printf("\n1. Show movies released in the specified year\n");
        printf("2. Show highest rated movie for each year\n");
        printf("3. Show the title and year of release of all movies in a specific language\n");
        printf("4. Exit from the program\n");
        printf("Enter a choice from 1 to 4: ");
        scanf("%d", &choice);

        // All of this handles the users choice
        if (choice == 1) {
            int year;
            printf("Enter the year for which you want to see movies: ");
            scanf("%d", &year);
            show_movies_by_year(head, year);
        } else if (choice == 2) {
            show_highest_rated_by_year(head);
        } else if (choice == 3) {
            char language[20];
            printf("Enter the language for which you want to see movies (First Letter must be capitalized!): ");
            scanf("%s", language);
            show_movies_by_language(head, language);
        } else if (choice == 4) {
            break;
          // Invalid Choice
        } else {
            printf("You entered an incorrect choice. Try again.\n");
        }
    }
}

// Main function to run the program
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <csv_file>\n", argv[0]);
        return 1;
    }

    Movie *head = NULL;             // Initalize the linked list
    process_file(argv[1], &head);   // Process the CSV file and populate the linked list
    display_menu(head);             // Display the menu and handle user choices
    free_movies(head);              // Free the linked list when done

    return 0;
}
