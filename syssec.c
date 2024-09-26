#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<time.h>
#include<unistd.h>
#include<math.h>

// struct for the user
struct User{
    char username[128];
    char date_of_birth[11];
    char pass_file[11];
    char pass[10][21];
};

//checking for special characters
const char special_character[] = ".@!#$%^&*-_";

// Loading previous passwords from pass_file
void get_password(char pass_file[11],char pass[10][21],int* n){
    FILE* file = fopen(pass_file,"r");
    if(!file){
        printf("Password File doesnt exist\n");
        exit(1);
    }
    while (fscanf(file,"%s",pass[*n])!=EOF && *n<10) {
        (*n)++;
    }
    fclose(file);
}

// Getting Account info from masterfile.txt
void get_user_info(struct User users[],int *n){
    FILE* file = fopen("masterfile.txt","r");
    if(!file){
        printf("MasterFile could not be opened\n");
        exit(1);
    }
    fscanf(file,"%s %s %s",users[*n].username,users[*n].date_of_birth,users[*n].pass_file);
    while(fscanf(file,"%s %s %s",users[*n].username,users[*n].date_of_birth,users[*n].pass_file)!=EOF){
        (*n)++;
    }
    fclose(file);
}


int special_character_check(char pass[]){
  for(int i=0;i<strlen(pass);i++){
        if(strchr(special_character,pass[i])){
          return 1; 
        }
  }
  return 0;
}


//Saving new passwords to the file
void save_passwords(char* filename,char passwords[10][21],int prev){
    FILE* file = fopen(filename,"w");
    if(!file){
        printf("File couldnt be opened\n");
        exit(1);
    }
    for(int i=0;i<prev;i++){
        fprintf(file,"%s\n",passwords[i]);
    }
    fclose(file);
}


// for printing the waiting timer
void delay_timer(int time) {
    for (int i=time;i>0;i--) {
        printf("\rWait for %d seconds....",i);
        fflush(stdout);
        sleep(1);
    }
    printf("\r \r");
}


void to_lowercase(char *s) {
    for (int i=0;s[i];i++){
        s[i]=tolower((unsigned char)s[i]);
    }
}


// checking the password
int valid_password_checker(char new_pass[],struct User user,int prev,int attempt,char password[][21]){
    int possible_violate[8];
    for(int i=0;i<8;i++)possible_violate[i]=0;

    // Checking condition for length > 12
    if(strlen(new_pass)<12){
      possible_violate[0]=1;
    }

    // Checking condition for atleast 1 uppercase
    int upper=0;
    for(int i=0;i<strlen(new_pass);i++){
        if(isupper(new_pass[i])){
          upper=1;break;
        }
    }

    if(!upper){
      possible_violate[1]=1;
    }

    // Checking condition for atleast 1 lowercase
    int lower=0;
    for(int i=0;i<strlen(new_pass);i++){
        if(islower(new_pass[i])){
          lower=1;
          break;
        }
    }
    if(!lower){
      possible_violate[2]=1;
    }

    // Checking condition for atleast 1 digit
    int digit=0;
    for(int i=0;i<strlen(new_pass);i++){
        if(isdigit(new_pass[i])){
          digit=1;
          break;
        }
    }

    if(!digit){
      possible_violate[3]=1;
    }
  
    // Checking condition for atleast 1 special character
    if(!special_character_check(new_pass)){
      possible_violate[4]=1;
    }
  
    // Checking condition if password contains more than 4 characters from previous passwords
    int flag=0;
    int maximum_found=0;
    for (int i=0;i<prev;i++){
        int current_found = 0;
        for (int j = 0; j < strlen(password[i]); j++) {
            for (int k = 0; k < strlen(new_pass); k++) {
                int dig = 0;
                // Comparing the substrings from password[i][j] and new_pass[k]
                while (j + dig < strlen(password[i]) && k + dig < strlen(new_pass) && tolower(password[i][j + dig]) == tolower(new_pass[k + dig])) {
                    dig++;
                }
                if (dig > current_found){
                  current_found = dig;
                }
            }
        }
      
        if (current_found > maximum_found) {
            maximum_found = current_found;
        }
        current_found = 0;
    }
    if(maximum_found>4){
      possible_violate[5]=1;
    }

    for(int i=0;i<strlen(new_pass);i++){
      if (!isalnum(new_pass[i]) && (new_pass[i] != '.') && (new_pass[i] != '@') && (new_pass[i] != '#') && (new_pass[i] != '$') && (new_pass[i] != '%' ) && (new_pass[i] != '!') && (new_pass[i] != '^') && (new_pass[i] != '&') && (new_pass[i] != '_') && (new_pass[i] != '*') && (new_pass[i] != '-') ){
        possible_violate[5] = 1;
      }
    }

  
    // Checking if password contains username or firstname or surname
    char fname[128],surname[128];
    sscanf(user.username,"%[^.].%s",fname,surname);
    int first_and_second=0,first=0,second=0;
    char lower_new_password[21];
    char lower_first_name[128];
    char lower_surname[128];
  
    strcpy(lower_surname, surname);
    strcpy(lower_new_password, new_pass);
    strcpy(lower_first_name, fname);
    to_lowercase(lower_new_password);
    to_lowercase(lower_first_name);
    to_lowercase(lower_surname);

  
    //strstr function is used to find the first occurrence of a substring within another string.
    if (strstr(lower_new_password,lower_first_name) && strstr(lower_new_password,lower_surname)){
      first_and_second=1;
    }
    else if(strstr(lower_new_password,lower_first_name)){
      first=1;
    }
    else if(strstr(lower_new_password,lower_surname)){
      second=1;
    }
    if(first_and_second||first||second){
      possible_violate[6]=1;
    }
  
    // Password contains more than 3 consecutive digits of date_of_birth
    char date_of_birth_digits[9];
    sprintf(date_of_birth_digits,"%c%c%c%c%c%c%c%c",user.date_of_birth[0],user.date_of_birth[1],user.date_of_birth[3],user.date_of_birth[4],user.date_of_birth[6],user.date_of_birth[7],user.date_of_birth[8],user.date_of_birth[9]);
    // printf("%s", date_of_birth_digits);
    int len=strlen(new_pass); 
    int maxi=0;

    if(strlen(new_pass) > 3){
  
    for(int i=0;i<strlen(new_pass) - 3;i++){
      if (new_pass[i] == date_of_birth_digits[0] && new_pass[i+1] == date_of_birth_digits[1] && new_pass[i+2] == date_of_birth_digits[2] && new_pass[i+3] == date_of_birth_digits[3]){
        maxi = 4;
      }
      if (new_pass[i] == date_of_birth_digits[1] && new_pass[i+1] == date_of_birth_digits[2] && new_pass[i+2] == date_of_birth_digits[3] && new_pass[i+3] == date_of_birth_digits[4]){
        maxi = 4;
      }
      if (new_pass[i] == date_of_birth_digits[2] && new_pass[i+1] == date_of_birth_digits[3] && new_pass[i+2] == date_of_birth_digits[4] && new_pass[i+3] == date_of_birth_digits[5]){
        maxi = 4;
      }
      if (new_pass[i] == date_of_birth_digits[3] && new_pass[i+1] == date_of_birth_digits[4] && new_pass[i+2] == date_of_birth_digits[5] && new_pass[i+3] == date_of_birth_digits[6]){
        maxi = 4;
      }
      if (new_pass[i] == date_of_birth_digits[4] && new_pass[i+1] == date_of_birth_digits[5] && new_pass[i+2] == date_of_birth_digits[6] && new_pass[i+3] == date_of_birth_digits[7]){
        maxi = 4;
      }
    }
    }

    //listing the violations
      
    if(maxi>=4){
      possible_violate[7]=1;
    }
      
    if(attempt<4){
        if(possible_violate[0]){
          printf("Password does not contain a minimum of 12 characters.\n");
        }
        if(possible_violate[1]){
          printf("Password does not contain at least one uppercase letter.\n");
        }
        if(possible_violate[2]){
          printf("Password does not contain at least one lowercase letter.\n");
        }
        if(possible_violate[3]){
          printf("Password does not contain at least one digit.\n");
        }
        if(possible_violate[4]){
          printf("Password does not contain at least one of the allowed special characters.\n");
        }
        if(possible_violate[5]){
          printf("Password contains %d characters consecutively similar to one of the past 10 passwords OR the Password contains an unallowed character\n",maximum_found);
        }
        if(possible_violate[6]&&first_and_second){
          printf("Password contains name and surname portions of username.\n");
        }
        if(possible_violate[6]&&first){
          printf("Password contains name portion of the username.\n");
        }
        if(possible_violate[6]&&second){
          printf("Password contains surname portion of username.\n");
        }
        if(possible_violate[7]){
          printf("Password contains %d digits consecutively similar to the date of birth.\n",maxi);
        }
    }
      
    for(int i=0;i<8;i++){
        if(possible_violate[i]==1){
            return 0;
        }
    }
    return 1;
}

  
int main(){
    struct User users[11];
    int no_of_users=0;
    get_user_info(users,&no_of_users);
    char username[10],pass[21];
    int authorization = 0;

    printf("Enter username: ");
    scanf("%s",username);
    //print username
    //printf("%s\n",username);
    int idx = -1;
  
    for(int i=0;i<no_of_users;i++){
        if(strcmp(users[i].username,username)==0){
            idx=i;
            break;
        }
    }

    if(idx==-1){
        printf("Username not found.\n");
        return 1;
    }

  
    char pass_file[11];
    strcpy(pass_file,users[idx].pass_file);
    char passwords[10][21];
    int number_of_passwords=0;
    get_password(pass_file,passwords,&number_of_passwords);
  
    // checking the number of login attemots
    int num_of_attempts=0;
    while(num_of_attempts<3 && !authorization){
        printf("Enter password: ");
        scanf("%s",pass);

        if(strcmp(pass,passwords[0])==0){
            authorization=1;
            printf("Login Successful.\n");
        }else{
            printf("Wrong password! Enter password again:\n");
            num_of_attempts++;
        }
    }

  
    if(!authorization){
        printf("Wrong password entered 3 times. Application exiting...\n");
        return 1;
    }

    // The number of attempts password can be changed
    int valid = 0;
    num_of_attempts = 0;
  
    while(num_of_attempts<4 && !valid){
        printf("Enter your new password (%d attempt): ",num_of_attempts+1);
        scanf("%s",pass);
      
        if(valid_password_checker(pass,users[idx],number_of_passwords,num_of_attempts+1,passwords)){
            printf("Password changed successfully.\n");
            for(int i=9;i>0;i--){
                strcpy(passwords[i],passwords[i-1]);
            }
          
            strcpy(passwords[0],pass);
            save_passwords(pass_file,passwords,number_of_passwords<10?number_of_passwords+1:10);
            valid=1;
        }
        else{
            num_of_attempts++;
            if(num_of_attempts==4){
              break;
            }
            if(num_of_attempts==1){
                delay_timer(8);
            }
            else if(num_of_attempts==2){
                delay_timer(16);
            }
            else if(num_of_attempts==3){
                delay_timer(32);
            }
        }
    }

    
    // condition if not valid
    if(!valid){
        printf("Failed to enter a valid password in 4 attempts.\n");
    }
    
    return 0;
}