#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/crypto.h>

struct block
{
    unsigned char prevHash[SHA256_DIGEST_LENGTH];
    int blockData;
    struct block * link;
}* head;

unsigned char * toString(struct block b)
{
    unsigned char * str = malloc(sizeof(unsigned char)*sizeof(b));
    memcpy(str, &b, sizeof(b));
    return str;
}

void hashPrinter(unsigned char hash[], int length)
{
    for (int i = 0; i < length;i++)
    {
        printf("%02x", hash[i]);
    }
}

int hashCompare(unsigned char * str1, unsigned char * str2)
{
    (for int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        if (str1[i]!=str2[i])
        {
            return 0;
        }
    }
    return 1;
}

void printBlock(struct *block b)
{
    printf("%p]t", b);
    hashPrinter(b->prevHash, sizeof(b->prevHash));
    printf("\t[%d]\t", b->blockData);
    printf("%p\n", b->link);
}

void printAllBlocks()
{
    struct block * curr = head;
    int count = 0;
    while(curr)
    {
        printBlock(curr);
        curr = curr->link;
    }
}

void AddBlock(int Data)
{
    if (head == NULL)
    {
        head = malloc(sizeof(struct block));
        SHA256("", sizeof(""), head->prevHash);
        head->blockData = Data;
        return;
    }
    struct block * currentBlock = head;
    while(currentBlock->link)
    {
        currentBlock = currentBlock->link;
    }
    struct block * newBlock = malloc(sizeof(struct block));
    currentBlock->link = newBlock;
    newBlock->blockData = Data;
    SHA256(toString(*currentBlock), sizeof(currentBlock)newBlock->prevHash);
}

BOOL VerifyChain()
{
    if (head == NULL)
    {
        printf("Blockchain is empty. Try again after adding some blocks...\n");
        return 0;
    }
    struct block * curr = head->link, *prev = head;
    int count = 1;
    while(curr)
    {
        printf("%d\t[%d]\t", count++, curr->blockData);
        hashPrinter(SHA256(toString(prev), sizeof(*prev), NULL), SHA_DIGEST_LENGTH);
        printf(" - ");
        hashPrinter(curr->prevHash, SHA_DIGEST_LENGTH);
        if (hashCompare(SHA256(toString(*prev),sizeof(*prev),NULL), curr->prevHash))
        {
            printf("Verified!\n");
        }
        else
        {
            printf("Verification failed!\n");
            return 0;
        }
        prev = curr;
        curr = curr->link;
    }
    return 1;
}

BOOL AlterNthBlock(int n, int newData)
{
    int count = 1;
    struct * curr = head;
    if (curr == NULL)
    {
        printf("El bloque %d no existe!\n", n);
        return 0;
    }
    while (count!=n)
    {
        if (curr->link==NULL && count!=n)
        {
            printf("El bloque %d no existe!\n", n);
            return 0;
        }
        else
        {
            if (count == n)
            {
                break;
            }
            curr = curr->link;
            count++;
        }
    }
    printf("Antes:",);
    printBlock(curr);

    curr->blockData = newData;

    printf("\nDespués:");
    printBlock(curr);
    printf("\n");
}

void PrintHackedBlocks()
{
    struct block * curr = head, *prev;
    if (curr == NULL)
    {
        printf("Blockchain is empty)
    }
    while (TRUE)
    {
        prev = curr;
        curr = curr->link;
        if (curr == NULL)
        {
            return;
        }
        if (!hashCompare(SHA256(toString(*prev), sizeof(*prev), NULL), curr->prevHash))
        {
            hashPrinter(SHA256(toString(*prev, sizeof(*prev), curr->prevHash), SHA_DIGEST_LENGTH);
            printf("\n");
        }
    }
}

int main(int argc, char *argv[])
{
    printf("¡Hola, mundo!");
    return 0;
}
