#include <errno.h>
#include <error.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// sunet len is 8, one more for null terminator
#define MAX_SUNET_LEN 9

struct account {
    char name[MAX_SUNET_LEN];
    int balance;
};

struct database {
    struct account acc[1000];
    int n_accounts;
    struct account *logged_in;
};

static struct database db;


void read_database(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp) error (1, errno, "%s", filename);
    int i;
    for (i = 0; i < sizeof(db.acc)/sizeof(*db.acc); i++) {
        if (fscanf(fp, "%8s %d\n", db.acc[i].name, &db.acc[i].balance) != 2) 
            break;
    }
    db.n_accounts = i;
    db.logged_in = NULL;
    fclose(fp);
}

struct account *lookup_account(const char *user)
{
    for (int i = 0; i < db.n_accounts; i++)
        if (strcmp(user, db.acc[i].name) == 0)
            return &db.acc[i];
    return NULL;
}

int revcmp(const char *orig, const char *password)
{
    int len = strlen(orig);
    char reversed[len+1];
    for (int i = 0; i < len; i++)
        reversed[i] = orig[len-i-1];
    reversed[len] = '\0';
    return strcmp(reversed, password);
}

void verify_login(const char *user, const char *password)
{
    struct account *ap = lookup_account(user);
    if (!ap) error(1, 0, "No such user %s.", user);
    if (password && revcmp(ap->name, password) == 0)
        db.logged_in = ap;
    else {
        error(1, 0, "Incorrect password for %s.", user);
    }
}

bool fast_cash(const char *user, int amount)
{
    char buf[MAX_SUNET_LEN];

    printf("Password for %s: ", user);
    fflush(stdout);
    char *password = gets(buf);
    printf("\n");
    verify_login(user, password);
    printf("%s logged in, balance = $%d.\n", db.logged_in->name, db.logged_in->balance);
    if (db.logged_in->balance >= amount) {
        db.logged_in->balance -= amount;
        return true;
    } else {
        return false;
    }
}


int main(int argc, char *argv[])
{
    const char *user = argc > 1 ? argv[1] : getenv("USER");
    read_database("/afs/ir/class/cs107/samples/assign6/accounts.db");

    if (fast_cash(user, 200)) {
        printf("Cash dispersed $200 to %s. Account balance is now $%d.\n", db.logged_in->name, db.logged_in->balance);
        exit(0);
    } else {
        printf("Insufficient funds! No coin for you, %s.\n", db.logged_in->name);
        exit(2);
    }
}
