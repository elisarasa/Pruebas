#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 60
#define DESCRIPTION_MAX 250

#define ADMIN 1
#define CLIENT 2
#define RESEARCHER 3

#define NOT_FOUND -1

// store payment information
typedef struct {
    char card[MAX];
    char pin[5];
} PaymentInfo;

//store dates
typedef struct {
    int year;
    int month;
    int day;
    int hour;
    int minute;
} Date;

//store crew information
typedef struct {
    int id;
    char name[MAX];
    Date birth_date;  //it said to store age but this way it's always updated no?
    int contact_number;
    char color[MAX];    //the color assigned by the spaceship they work in
    char *roles[MAX];   //only support crew members have roles, they can have multiple
} Crew;

typedef struct {
    char name[MAX];
    int damaged; //(1 = YES and 0 = NO)
} Component;

//store spaceship information
typedef struct {
    int id;
    char name[MAX];
    int crew_capacity;
    int passenger_capacity;
    char crew_color[MAX];
    Component communication_system;
    Component engine;
    Component navigation_system;
    Component propulsion_system;
    Component *extra_modules;
    int scientific_instrumentation;
} Spaceships;

//store trip information
typedef struct {
    int id;
    char *itinerary[MAX];
    char departure_point[MAX];
    char destination[MAX];
    Date departure_date;
    Date arrival_date;
    Date duration;
    Spaceships spaceship;
} Trips;

// store project information
typedef struct {
    int id;
    int responsible_id;
    char title[MAX];
    char description[DESCRIPTION_MAX];
    int status;
    float budget;
    float cost;
    int trip_id;
    int spaceship_id;
    int *researchers_id;
    // boolean to distinguish design proj and spatial trip projects
    int design_project;
    int spatial_trip;
} Projects;

//common user attributes
typedef struct {
    int id;
    char name[MAX];
    char password[MAX];
    char email[MAX];
    int type;
    char pin[9];
} User;

//researcher specific information
typedef struct {
    int id; //the id from common user attributes, used to relate them
    int num_projects;
    Projects *projects;  //could also be managed by max num of projects
} Researcher;

//client specific information
typedef struct {
    int id; //the id from common user attributes, used to relate them
    PaymentInfo payment;
    int num_trips;
    Trips *trip;
} Client;

int checkEmail(char email[], User user[], int user_number) {
    int i;
    int index = NOT_FOUND;
    for (i = 0; i < user_number; i++) {
        if (strcmp(user[i].email, email) == 0) {
            index = i;
        }
    }
    return index;
}

int registerUser(FILE *fp_users, int user_number, User *user) {
    char email[MAX];
    int success = 0;
    printf("Welcome to StellarSalleX\n");

    printf("Enter your email: ");
    scanf("%s", email);

    if (checkEmail(email, user, user_number) != NOT_FOUND) {
        printf("The account with email %s already exists\n", email);
    }
    else {
        strcpy(user[user_number].email, email);
        printf("Enter password: ");
        scanf("%s", user[user_number].password);

        printf("Enter your name (no tab spaces): ");
        scanf("%s", user[user_number].name);

        printf("What's your role in StellarSalleX?\n1) Admin\t2) Client\t3) Researcher\n");
        scanf("%d", &user[user_number].type);

        if (user[user_number].type == ADMIN) {
            printf("Enter PIN (6 characters): ");
            scanf("%s", user[user_number].pin);
        }
        else {
            strcpy(user[user_number].pin, 'noPin');
        }
        user[user_number].id = user_number;

        success = 1;
    }

    return success;
}


int main() {
    int access = 0;
    int program_status = 1;
    int user_number = 0;
    FILE *fp_users;
    User *user = NULL;

    while (program_status == 1) {
        printf("\t1. Login\n\t2. Register\n\t3. Exit\nAccess:");
        scanf("%d", &access);

        switch (access) {
            case 1:
                break;
            case 2:
                fp_users = fopen("users.txt", "r+");
                if (fp_users == NULL) {
                    printf("Error opening Users file\n");
                }
                else {
                    fscanf(fp_users, "There are %d users", &user_number);
                    user = (User *) malloc(sizeof(User) * (user_number + 1));
                    if (user == NULL) {
                        printf("Error allocating User memory");
                    }
                    else {
                        for (int i = 0; i < user_number; i++) {
                            fscanf(fp_users, "%s", user[i].email);
                            fscanf(fp_users, "%s", user[i].password);
                            fscanf(fp_users, "%s", user[i].name);
                            fscanf(fp_users, "%d", &user[i].id);
                            fscanf(fp_users, "%d", &user[i].type);
                            fscanf(fp_users, "%s", &user[i].pin);
                        }

                        if (registerUser(fp_users, user_number, user) == 1) {
                            printf("Successfully registered\n");

                            fseek(fp_users, 0, SEEK_SET);

                            fprintf(fp_users, "There are %d users\n", user_number + 1);

                            for (int i = 0; i < user_number + 1; i++) {
                                fprintf(fp_users, "%s %s %s %d %d %s\n", user[i].email, user[i].password, user[i].name, user[i].id, user[i].type, user[i].pin);
                            }
                        }
                    }


                    fclose(fp_users);
                    free(user);
                }
                break;
            case 3:
                program_status = 0;
            break;
            default:
                printf("Invalid access.\nPlease try again.\n");
        }
    }

    return 0;
}
