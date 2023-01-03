#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Car{
    char* plate;
    int mileage;
    int returnDate;
    struct Car* next;
};

typedef struct Car Car;

Car * loadDataFromFile(FILE *, Car* , Car* ,Car*, int); 
void printlist(Car *);
Car * makeInstance(char *, int , int );
Car * addBack(Car *, Car * );
void LowestFirst (Car **, Car * );
Car * search(Car *, char*);
void deleteNode(Car**, int);
void loadToFile(Car*,int);
void cost(int, float*);
void deleteList(Car *);

// gcc -std=c99 -Wall  LinkedLists.c -o  lists

int main(){

    int choice;
    Car* Available = NULL;
    Car* Rented = NULL;
    Car* AtWkshop = NULL;
    char value[30];  
    FILE *inFile = NULL;
    char userInput[30];
    int newMiles;
    Car *newNode = NULL;
    Car *val = NULL;
    int date;
    float moneyMade = 0;


   //load files first for option 7 

    Available = loadDataFromFile(inFile, Available, Rented, AtWkshop,1);
    Rented = loadDataFromFile(inFile, Available, Rented, AtWkshop,2);
    AtWkshop = loadDataFromFile(inFile, Available, Rented, AtWkshop,3);


do {
    //menu
    printf("1. Add a new car to the available-for-rent list\n");
    printf("2. Add a returned car to the available-for-rent list\n");
    printf("3. Add a returned car to the repair lsit\n");
    printf("4. Transfer a car from the repair lsit to the available-for-rent list\n");
    printf("5. Rent the first available car\n");
    printf("6. Print all the lists\n");
    printf("7. Quit\n");
 
    printf("Chose a nemu option: ");
    fgets(value, 30, stdin);
    choice = atoi(value);

    switch (choice)
    {
    case 1:
    //adding a new car to list

        printf("\nYou have selected option 1.\nPlease enter plate number (7 character limit with spaces): ");
        scanf("%s", userInput);
        getchar();

        if(strlen(userInput) > 7){
            printf("Over character limit\n");
            break;
        }
        //check if plate is unique in all 3 lists
        val = search(Available, userInput);
        
        if(val != NULL){
            printf("Car with plate number already exists. Input is invalid\n");
            break;
        }
       
        val = search(Rented, userInput);
        
        if(val != NULL){
            printf("Car with plate number already exists. Input is invalid\n");
            break;
        }
        
        val = search(AtWkshop, userInput);

        if(val != NULL){
            printf("Car with plate number already exists. Input is invalid\n");
            break;
        }


        printf("\nPlease enter the mileage: ");
        scanf("%d", &newMiles);
        getchar();

        //mileage cannot be a negative number
        if (newMiles < 0){
            printf("Not valid input for mileage\n");
            break;
        }

        //make node
        newNode = makeInstance(userInput, newMiles, 0);
        //sort node
        LowestFirst(&Available, newNode);  

        printf("\n\n**New car was added**\n\n");


        break;
    case 2:

        //move car from rented list to available list

        printf("\nYou have selected option 2.\nPlease enter plate number of the "
        "car you are returning: ");
        scanf("%s", userInput);
        getchar();

        // gets the node back if car they are returning exists in rented list
        val = search(Rented, userInput); 
        // checks for null
        if (val == NULL)
        {
            printf("Not a valid plate number\n");
            break;
        }
        // get the new mileage
        printf("\nPlease enter the mileage: ");
        scanf("%d", &newMiles);
        getchar();

        //mileage cannot be 0 or negative
        if (newMiles <= 0){
            printf("Not valid input for mileage\n");
            break;
        }
        /// new miles is the new added mileage
         int allMiles = val->mileage + newMiles;

        char numplate[20];
        strcpy(numplate,val->plate);

        //create node
        newNode = makeInstance(numplate,allMiles,0);
        //delete node from returned list
        deleteNode(&Rented, val->mileage);
        //creatd node is moved to available list
        LowestFirst(&Available, newNode);
        //calculate cost
        cost(newMiles, &moneyMade);

        printf("\n**Added returned car to available-for-rent-list**\n\n");
        
    
        break;    
    case 3:

        //move car from rented list to repair list

        printf("\nYou have selected option 3.\nPlease enter plate number of the "
        "car you are returning for repair: ");
        scanf("%s", userInput);
        getchar();

        // gets the node back if car they are returning exists in rented list
        val = search(Rented, userInput); 
        // checks for null
        if (val == NULL)
        {
            printf("not a valid plate number\n");
            break;
        }
        // get the new mileage
        printf("\nPlease enter the mileage: ");
        scanf("%d", &newMiles);
        getchar();

        //mileage cannot 0 or negative
        if (newMiles <= 0){
            printf("Not valid input for mileage\n");
            break;
        }

        /// new miles is the new added mileage
         int totalMiles = val->mileage + newMiles;

         char numberplate[20];
         strcpy(numberplate,val->plate);
        
        //create node
        newNode = makeInstance(numberplate,totalMiles,0);
        //delete node from rented list
        deleteNode(&Rented, val->mileage);
        //move created node to repair list
        LowestFirst(&AtWkshop, newNode);

        //calculate cost
        cost(newMiles, &moneyMade);

        printf("\n**Added returned car to repair-list**\n\n");
         

        break;
    case 4:

        //transfer car from repar to available list

        printf("\nYou have selected option 4.\nPlease enter plate number of the "
        "car you want to transfer from repair list to available-for-rent list: ");
        scanf("%s", userInput);
        getchar();

        // gets the node back if car they are exists exists in repair list
        val = search(AtWkshop, userInput); 
        // checks for null
        if (val == NULL)
        {
            printf("not a valid plate number\n");
            break;
        }

        char plateOfCar[20];
        strcpy(plateOfCar,val->plate);

        int milesOnCar = val->mileage;

        //create node
        newNode = makeInstance(plateOfCar,milesOnCar,0);
        //delte node from repair list
        deleteNode(&AtWkshop, val->mileage);
        //move created node to available list
        LowestFirst(&Available, newNode);

        printf("\n\n**Car was transferred from repair list to available-for-rent-list**\n\n");


        break;
    case 5:
    
    //rent a car

    //check if list has cars available for rent
    if (Available == NULL){
        printf("**No car is available for rental.**\n");
        break;
    }

    printf("\nYou have selected option 5. Enter the date of return in format YYYYMMDD: ");
    scanf("%s", userInput);
    getchar();

    //date error check
    if(strlen(userInput) != 8){
        printf("Invalid input date format\n");
        break;
    }

    char numPlate[20];

    strcpy(numPlate, Available->plate);

    int driven = Available->mileage;
    date = atoi(userInput);

    //create node
    newNode = makeInstance(numPlate, driven, date);
    //delete node from available list
    deleteNode(&Available, driven);
    //move created node to rented list
    LowestFirst(&Rented, newNode);

    printf("\n\n");

    printf("You have rented a car.\nPlate Number:%s\nMileage:%d\nReturn Date:%d\n", numPlate,driven,date);
 
        break;
    case 6:

    //print all lists

    printf("\nFormat: PlATE  MILEAGE  DATE\n\n");
    printf("Available List\n");
    printlist(Available);

    printf("\n");
    printf("Rented List\n");
    printlist(Rented);

    printf("\n");
    printf("Repair List\n");
    printlist(AtWkshop);

    printf("\n");

        break;
    default:
        //print total money made
        //load data to file
        printf("The profit made was $%.2f\n", moneyMade);
        loadToFile(Available,1);
        loadToFile(Rented,2);
        loadToFile(AtWkshop,3);
        break;

    }

} while (choice != 7);

    //free lists
    deleteList(Available);
    deleteList(Rented);
    deleteList(AtWkshop);

return 0;

}

//Free memory
void deleteList(Car *list){

    Car *temp;

    while(list != NULL){
        temp = list;
        list = list->next;
        free(temp);
        //list is freed

    }

}

//Calculate and print cost
void cost(int carDriven, float *val){

    float miles = carDriven;

    //fixed cost of 80 dollars
    if (carDriven >=1 && carDriven <= 200){
        *val = *val + 80;
        printf("The cost is $80.00.\n");
    }
    else{
    
        //if mileage is over 200 
        miles = miles - 200;
        float cost = miles * 0.15;
        float total = cost + 80.00;
        printf("The cost is $%.2f\n", total);
        //keep count of profit
        *val = *val + total;
        
    }
}

//Loads data to file
void loadToFile(Car *list, int num){

    FILE *inFile = NULL;

    if (num ==1){
        //available list
        inFile = fopen("lists.txt","w");
    }

    else if (num ==2){
        //rented list
        inFile = fopen("lists2.txt","w");
    }

    else if (num ==3){
        //repair list
        inFile = fopen("lists3.txt","w");
    }

    if (inFile == NULL){

        printf("File could not open to save data");
        exit(1);
    }
    else {

            while(list != NULL){
                //print data to file
                fprintf(inFile, "%s %d %d", list->plate, list->mileage, list->returnDate);
                fprintf(inFile,"\n");
                list = list->next;
            }
    }

    //close file
    fclose(inFile);

}

//Delete nodes 
void deleteNode(Car** list, int milesCar){

    Car* temp = NULL;
    Car* prev;
    //make temp equal to list
    temp = *list;

    //if mileage is found in head
    if(temp != NULL && temp->mileage == milesCar){
        //list is equal to next node
        *list = temp->next;
        //previous node is freed
        free(temp);
    }else{
        //when node is found, does not enter while loop
        while(temp != NULL && temp->mileage != milesCar){
            prev = temp;
            temp = temp->next;
        }

        //make it equal to next node and frees previous nodess
        prev->next = temp->next;
        free(temp);

    
}
}

//Searches for plate number and retuns that node, if not found, returns null
Car * search(Car *list, char *plateNum){

    //Returns null if list contains nothing
    if (list == NULL){
        return NULL;
    }

    while (list != NULL ){
        //if plate is found in list, returns node
        if ((strcmp(list->plate, plateNum) == 0)){
            return list;
        }
        list = list->next;

    }
 
 
    return NULL;

}

//Loads data from file to make lists and returns lists
Car * loadDataFromFile(FILE *inFile, Car* Available, Car* Rented,Car* AtWkshop, int num){

    Car* tempData;
    char tempPlate[50];
    int tempMile =0;
    int tempRet = 0;


    if (num == 1){
    //available list
        inFile = fopen("lists.txt","r");

        if (inFile == NULL){
            printf("Not successful, file for available list could not be opened");
        } 
    }
    else if (num == 2){
    //rented list
        inFile = fopen("lists2.txt","r");

        if (inFile == NULL){
            printf("Not successful, file for rented list could not be opened");
        } 
    }
    else if (num == 3){
        //repair list
        inFile = fopen("lists3.txt","r");

        if (inFile == NULL){
            printf("Not successful, file for available repair list could not be opened");
        } 
    }

    //read from file
    while (fscanf(inFile,"%s %d %d",tempPlate,&tempMile,&tempRet) == 3){    
        
        //make node
        tempData = makeInstance(tempPlate,tempMile,tempRet);
  
        if(num == 1){
            //link list
            Available = addBack(Available,tempData);
        }
        else if (num == 2){
            Rented = addBack(Rented,tempData);
        }
        else if (num == 3){
            AtWkshop = addBack(AtWkshop, tempData);
        }
    }

    fclose(inFile);


    //returns lists
    if(num == 1){
        return Available;
    }
    else if(num == 2){
        return Rented;
    }
    else if(num == 3){
        return AtWkshop;
    }

    return NULL;
}

//Adds to an already sorted list
void LowestFirst (Car ** list, Car * data){

    //Check for date (available list)
    if(data->returnDate == 0){

    //If list is null makes head or if head mileage is greater, makes new head
    if(*list == NULL || (*list)->mileage >= data->mileage){

        data->next = *list;
        *list = data;
        return;
    }

    Car *node = *list;
    //check to if current node is less than in mileage
    while(node->next != NULL && node->next->mileage < data->mileage){
        node = node->next;
    }

    data->next = node->next;
    node->next = data;

    }
    //Check for date (rented list)
    else if(data->returnDate != 0){
    
    //If list is null makes head or if head mileage is greater, makes new head
    if(*list == NULL || (*list)->returnDate >= data->returnDate){

        data->next = *list;
        *list = data;
        return;
    }

    Car *node = *list;
     //check to if current node is less than in date
    while(node->next != NULL && node->next->returnDate < data->returnDate){
        node = node->next;
    }

    data->next = node->next;
    node->next = data;

    }

}

//Make node of a car
Car * makeInstance(char *num_plate, int mile, int ret){
     
      //get plate length
      int len = strlen(num_plate);

      Car * autoptr = malloc(sizeof(Car));

      if(autoptr == NULL){
          return NULL;
      }
     //initialize to length of plate
      autoptr->plate = malloc(sizeof(char) * len);

      if(autoptr->plate == NULL){
          return NULL;
      }
    
      //Copy information    
      autoptr->mileage = mile;
      autoptr->returnDate = ret;
      strcpy(autoptr->plate, num_plate);

      autoptr->next = NULL;

      return autoptr;
  }

//Links list and returns null if list is null
Car * addBack(Car *list, Car * record){
    
    if(list != NULL && record != NULL){
        Car * temp = list;
        //if list is not null, equal to next
        while(temp->next != NULL){
            temp = temp->next;
        }
        temp->next = record;
        //returns list
        return list;
    }
    //returns the node if list is empty
    if(list == NULL && record != NULL){
        return record;
    }
    return NULL;
    }

//Print linked list
void printlist(Car *list){

    Car * temp = list;
    
    while(temp != NULL){
        printf("%s  %d  %d\n", temp->plate, temp->mileage, temp->returnDate);
        temp = temp->next;
    }
}
