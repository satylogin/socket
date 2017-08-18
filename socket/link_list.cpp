/*
 * Code author - Satyarth Agrahari, Program - link list 
 * ----------------------------------------------------
 * The following is an link list implementation in c language.
 * The program has a built in driving main function, although
 * the user is requested to change it accrding to his/her wish.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR -99999999 // the error value

struct node {
	int data;//to store data
	struct node *next;//to store address of next node
};

//function prototype declarations

struct node* new_node(int); //to create new node
void head(struct node**, int); //to insert data at the head of the list
void tail(struct node **, int); // to insert data at the tail of the link list
void print(struct node *); //to print the values in list
int delete_head(struct node**); //to delete head node
int delete_after(struct node*); //to delete after a given node
void insert_after(struct node*, struct node *); //to insert data after the given node

int main()
{
	int data, index, check;
	char str[30];
	scanf("%s", str);

	//the main pointer to point at the head of the link list
	struct node *root = NULL;

	//driver program, user has to type exit to stop the program
	//user can also modyfy the driver program according to his wish.
	while (strcmp(str, "exit") != 0) {
		if (strcmp(str, "head") == 0) {
			scanf("%d", &data);
			head(&root, data);
		} else if (strcmp(str, "print") == 0) {
			print(root);
		} else if (strcmp(str, "delete_head") == 0) {
			check = delete_head(&root);
			if (check == ERROR) {
				printf("Cannot delete from an empty list\n");
			} else {
				printf("The data at head was %d\n", check);
			}
		} else if (strcmp(str, "tail") == 0) {
			scanf("%d", &data);
			tail(&root, data);
		}

		scanf("%s", str);
	} 

	return 0;
}

/* *
 * new_node()
 * -----------------------------------------------------
 * This function takes an integer argument.It creates a new
 * node and insert the value of data. The pointer points to 
 * NULL. The address of new node is returned back.
 */
struct node* new_node(int data)
{
	struct node *p;
	p = (struct node*)malloc(sizeof(struct node)); //allocating memory to pointer
	p->data = data; //putting in data
	p->next = NULL; //next points to NULL
	return p;
}

/* *
 * head()
 * -----------------------------------------------------
 * This function takes an integer argument and the address of 
 * the main pointer. It adds the element to the head of the list.
 * This function does not return any value.
 */
void head(struct node **root, int data)
{
	struct node *tmp = new_node(data);
	tmp->next = *root;
	*root = tmp;
}

/* *
 * tail()
 * -----------------------------------------------------
 * This function takes an integer argument and the address of 
 * the main pointer. It adds the element to the tail of the list.
 * This function does not return any value.
 */
void tail(struct node** root, int data)
{
	struct node *p, *tmp;
	p = *root;
	tmp = new_node(data);
	if (p == NULL) {
		*root = tmp;
	} else {
		while (p->next != NULL) {
			p = p->next;
		}
		p->next = tmp;
	}
}

/* *
 * print()
 * -----------------------------------------------------
 * This function prints the whole list.
 */
void print(struct node *p)
{
	//check whether the list is empty
	if (p == NULL) {
		printf("The list is empty\n");
	} else {
		while (p != NULL) {
			printf("%d ", p->data);
			p = p->next;
		}
		printf("\n");
	}
}

/* *
 * delete_head()
 * -----------------------------------------------------
 * this function deletes the head node ans return the data
 * that was present at the head of the list. If the list is empty
 * it returns an error value.
 */
int delete_head(struct node** head)
{
	if (*head != NULL) {
		struct node *p;
		p = *head;
		int x = (*head)->data;
		(*head) = (*head)->next;
		free(p);
		return x;
	}
	return ERROR;
}

/* *
 * delete_after()
 * -----------------------------------------------------
 * This function deletes the next node of a given node.
 * It also returns the value stored in the node.
 * In case there is no element at or after the node, an 
 * error value is returned by the function.
 */
int delete_after(struct node *p)
{
	if (p != NULL && p->next != NULL) {
		int data; // correction from prev
		struct node *q;
		q = p->next;
		data = q->data;// correction from prev
		p->next = q->next;
		free(q);
		return data; // correction from prev
	} else {
		return ERROR;
	}
}

/* *
 * insert_after()
 * -----------------------------------------------------
 * This function inserts a given node after the reference
 * node. In case of empty list, an error value is printed 
 * the screen.
 */
void insert_after(struct node* reference, struct node *tmp)
{
	if (reference != NULL) {
		tmp->next = reference->next;
		reference->next = tmp;
	} else {
		printf("Trying to insert data in empty list\n");
	}
}
