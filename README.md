# Proactive Password Checker Application

## Overview

This project implements a **Proactive Password Checker Application** that assists registered users of a system to securely change their passwords. The system stores information about each user, including their username, date of birth, and password history. This application ensures that passwords created by the users are strong by enforcing various security rules.

## Features

- The application allows existing registered users to log in and change their passwords.
- Passwords must meet specific security requirements to be deemed strong.
- Users are authenticated using their username and current password.
- The application prevents users from creating weak passwords by checking them against defined rules.
- A password history is maintained for each user to ensure that newly created passwords are sufficiently different from previous ones.
- Users have 3 attempts to create a valid password, with increasing backoff times for each failed attempt.

## System Details

- **Number of users**: `n` where `3 ≤ n ≤ 10`
- **User data stored**: Username, date of birth, and password history (stored in a file named `masterfile.txt`).
- **Password history**: The last 10 passwords of each user are stored in a password file (e.g., `rypass.txt` for user `ramesh.yadav`). If a user has changed their password fewer than 10 times, all entries are maintained; otherwise, the oldest password(s) are discarded.

### `masterfile.txt` Format

The `masterfile.txt` contains user information in the following format:

```
username         date_of_birth       password_file_name
ramesh.yadav     19-09-1985          rypass.txt
puja.bedi        23-08-1991          pbpass.txt
arun.kumar       01-11-1973          akpass.txt
anjali.sharma    30-01-1996          aspass.txt
```

### Password File Format

The password file stores up to 10 passwords in descending order of date created, with the most recent password first. Example:

```
2px34sa19h.fS
lkA@!o90a$5p
m.M90a21gth*k
xCXtimPOT23!p
Abcd.1234.*S
9091@asdfOOP$
```

### Password Creation Requirements

The following rules must be met for a password to be considered strong:

1. **R1**: Minimum length of 12 characters.
2. **R2**: At least one uppercase letter.
3. **R3**: At least one lowercase letter.
4. **R4**: At least one digit.
5. **R5**: At least one special character from the allowed set: `. @ ! # $ % ^ & * - _`
6. **R6**: No more than 4 consecutive characters from any of the last 10 passwords (case-insensitive).
7. **R7**: Password should not contain the name and/or surname portions of the username (case-insensitive).
8. **R8**: Password should not contain more than 3 consecutive digits from the user's date of birth.

### Authentication Flow

1. **Login**:
   - User is prompted to enter their username and password.
   - If the password is incorrect, the user gets 3 attempts before the application exits.

2. **Password Change**:
   - Once authenticated, the user is prompted to enter a new password.
   - If the password violates any requirements, the user is notified of the exact violations and given the chance to try again.
   - A backoff timer is enforced for each failed attempt (8 seconds after the first failure, 16 seconds after the second).

### Sample Prompts

- **Login**:
  ```
  Enter username:
  Enter password:
  Wrong password! Enter password again:
  ```

- **Password Creation**:
  ```
  Enter your new password (1st attempt):
  First attempt failed.
  Password does not contain at least one uppercase letter.
  Wait for x seconds...
  ```

## Error Messages

If any password requirement is violated, the corresponding error message is displayed:

- **R1 Violation**: Password does not contain a minimum of 12 characters.
- **R2 Violation**: Password does not contain at least one uppercase letter.
- **R3 Violation**: Password does not contain at least one lowercase letter.
- **R4 Violation**: Password does not contain at least one digit.
- **R5 Violation**: Password does not contain at least one of the allowed special characters.
- **R6 Violation**: Password contains `c` characters consecutively similar to one of the past 10 passwords.
- **R7 Violation**: 
  - Password contains the name portion of the username.
  - Password contains the surname portion of the username.
  - Password contains both the name and surname portions of the username.
- **R8 Violation**: Password contains `d` digits consecutively similar to the date of birth.

## Backoff Timers

- After the **1st failed attempt**, a timer counts down from 8 seconds before prompting for a new password.
- After the **2nd failed attempt**, a timer counts down from 16 seconds.
- After the **3rd failed attempt**, a timer counts down from 32 seconds before terminating the application.

## Files Provided

- `masterfile.txt` containing user data.
- Password files for each user (e.g., `rypass.txt`, `pbpass.txt`, etc.).

---

### How to Run

1. Ensure that `masterfile.txt` and the respective password files are present in the same directory as the program.
2. Run the application.
3. Follow the prompts to log in and change your password.

