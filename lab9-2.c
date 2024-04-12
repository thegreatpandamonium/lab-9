#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
	struct RecordType *next;
};

// Fill out this structure
struct HashType
{
	int numOfElements;
	int size;
	struct RecordType** arr;
	

};

// Compute the hash function
int hash(int x)
{
	return x % 23;

}

// parses input file to an ineger array
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
			pRecord->next = NULL;
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

	for (i = 0; i < pHashArray->size; ++i)
	{
		// Print index
		printf("Index %d: ", i);

		if (pHashArray->arr[i] != NULL)
		{
			struct RecordType *currentRecord = pHashArray->arr[i];
			while (currentRecord != NULL)
			{
				printf("%d, %c, %d ", currentRecord->id, currentRecord->name, currentRecord->order);
				currentRecord = currentRecord->next;
			}
			printf("\n"); 
		}
		else
		{
			printf("\n"); 
		}
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);
	// Your hash implementation

	struct HashType hashTable;
    hashTable.size = 23;
    hashTable.arr = (struct RecordType **)malloc(sizeof(struct RecordType *) * hashTable.size);
    if (hashTable.arr == NULL)
    {
        printf("Memory allocation failed\n");
        exit(-1);
    }

    for (int i = 0; i < hashTable.size; ++i)
    {
        hashTable.arr[i] = NULL;
    }

    // Insert records into hash table using chaining
    for (int i = 0; i < recordSz; ++i)
    {
        int index = hash(pRecords[i].id);
        if (hashTable.arr[index] == NULL)
        {
            hashTable.arr[index] = &pRecords[i];
        }
        else
        {
            struct RecordType *currentRecord = hashTable.arr[index];
            while (currentRecord->next != NULL)
            {
                currentRecord = currentRecord->next;
            }
            currentRecord->next = &pRecords[i];
        }
    }

    displayRecordsInHash(&hashTable, hashTable.size);

    // Free memory allocated for the hash table
    for (int i = 0; i < hashTable.size; ++i)
    {
        struct RecordType *currentRecord = hashTable.arr[i];
        while (currentRecord != NULL)
        {
            struct RecordType *temp = currentRecord;
            currentRecord = currentRecord->next;
            free(temp);
        }
    }
    free(hashTable.arr);
    free(pRecords);

    return 0;
}