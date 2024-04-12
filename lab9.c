#include <stdio.h>
#include <stdlib.h>
#define HASH_SIZE 23

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

// Fill out this structure
struct HashType
{
	struct RecordType* record; // Change: record is now a pointer to struct RecordType
	struct HashType* next;
};

// Compute the hash function
int hash(int x)
{
	int hashed_value;
	hashed_value = x%23;
	return hashed_value;
}

// Initializes our Hash
void initHash(struct HashType* hashTable, int hashSz)
{
	for(int i = 0 ; i < hashSz ; i++)
	{
		hashTable[i].next = NULL;
	}
}

// Inserting Records
void insertRecord(struct HashType* hashTable, int hashSize, struct RecordType record)
{
    int index = hash(record.id); // Calculate hash index

    // Create a new node for the record
    struct HashType* newNode = (struct HashType*)malloc(sizeof(struct HashType));
    if (newNode == NULL)
    {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    newNode->record = (struct RecordType*)malloc(sizeof(struct RecordType)); // Allocate memory for the record
    *(newNode->record) = record; // Copy the record data
    newNode->next = NULL;

    // If the bucket is empty, insert the record as the head
    if (hashTable[index].next == NULL)
    {
        hashTable[index].next = newNode;
    }
    else // If the bucket is not empty, append to the end of the list
    {
        struct HashType* temp = hashTable[index].next;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;

	for (i=0;i<hashSz;++i)
	{
		if(pHashArray[i].next == NULL) continue;
		printf("\nIndex %d: ", i);
		struct HashType* current = pHashArray[i].next;
		while(current!=NULL)
		{
			printf("%d, %c, %d -> ", current->record->id, current->record->name, current->record->order);
			current = current->next;
		}
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);
	struct HashType hashTable[HASH_SIZE];
    initHash(hashTable, HASH_SIZE);

    // Insert records into hash table
    for (int i = 0; i < recordSz; i++)
    {
        insertRecord(hashTable, HASH_SIZE, pRecords[i]);
    }

    // Display records in the hash table
    displayRecordsInHash(hashTable, HASH_SIZE);

    // Free allocated memory for records
    free(pRecords);

    return 0;
}
