#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TITLE_MAX 25
#define DESC_MAX 120
#define TAGS_MAX 10
#define TAGS_LEN 15
#define ID_LEN 13

// Array of strings
typedef char TAGS[TAGS_MAX][TAGS_LEN];

// Struct with string array and length
typedef struct {
  TAGS tags;
  int count;
} Tag_struct;

// Product structure
typedef struct {
  char title[TITLE_MAX];
  char description[DESC_MAX];
  char id[ID_LEN];
  Tag_struct tags;
  float price;
  int quantity;
  int numSold;
} Product;

// temp tags
TAGS sample_tags = {
  "food",
  "drug",
  "pet",
  "toiletry",
  "entertainment"
};

int verifyInput(char *input) {
  char response, dump;

  printf("You entered: %s\nIs that correct? [Y/n]\n", input);

  response = getchar();
  dump = getchar();

  return response == 'Y' || response == 'y' ? 1 : 0;
}

const char* getProductTitle(void) {

  char *title_p, title[TITLE_MAX];
  title_p = title;

  printf("Enter a name for new product: (Max 15 chars.)\n");
  fgets(title_p, TITLE_MAX + 1, stdin);

  if (title[strlen(title) - 1] == '\n')
    title[strlen(title) - 1] = '\0';

  return title_p; 
}

const char* getProductDesc(char *title) {

  char *desc_p, desc[DESC_MAX];
  desc_p = desc;

  printf("Enter a description for %s: \n", title);
  fgets(desc_p, DESC_MAX + 1, stdin);

  if (desc[strlen(desc) - 1] == '\n')
    desc[strlen(desc) - 1] = '\0';

  return desc_p;
}

const char* generateID(char *title) {

  int i;
  char *id_str_p, id_string[ID_LEN];
  id_str_p = id_string;

  id_string[0] = title[0];
  id_string[1] = title[2];

  for (i = 0; i < ID_LEN - 1; i++) {
    id_string[i + 2] = (rand() % 9) + '0';
  }
  
  return id_str_p;
}

const char* getProductID(char *title) {

  char *id_p, id[ID_LEN];
  id_p = id;

  printf("Generating ID for %s...\n", title);
  
  strcpy(id, generateID(title));

  return id_p;
}

Tag_struct* getProductTags(void) {

  Tag_struct prod_tags;

  char current_tag[TAGS_LEN];
  prod_tags.count = 0;

  int *count_p = &prod_tags.count;

  int status = 1;
  while (status) {
    printf("Enter tag for the product: \n");
    fgets(current_tag, TAGS_LEN, stdin);

    if (current_tag[strlen(current_tag) - 1] == '\n')
      current_tag[strlen(current_tag) - 1] = '\0';

    strcpy(prod_tags.tags[prod_tags.count], current_tag);
    prod_tags.count++;

    printf("Any more tags for this product? [Y/n]\n");

    char response, dump;
    response = getchar();
    dump = getchar();

    status = response == 'y' || response == 'Y' ? 1 : 0;
  }

  Tag_struct *prod_tags_p = &prod_tags;

  return prod_tags_p;
}

Product createProduct(void) {
  Product product;
  
  // Get product name from user
  // strcpy(product.title, getProductTitle()); - COMPLETE

  // Get product description from user
  // strcpy(product.description, getProductDesc(product.title)); -CMPTE

  // Generate product ID
  strcpy(product.title, "test_prod");
  strcpy(product.description, "test_description");
  strcpy(product.id, getProductID(product.title));

  Tag_struct prod_tags = *getProductTags();
  memcpy(&product.tags, &prod_tags, sizeof(prod_tags));

  printf("title       : %s\n", product.title);
  printf("description : %s\n", product.description);
  printf("ID          : %s\n", product.id);  
  printf("Tags        : \n");

  int i;
  for (i = 0; i < prod_tags.count; i++) {
    printf("\t%d. - %s\n", i + 1, product.tags.tags[i]);
  }

  return product;
}

int main() {
  Product product = createProduct();
  Product *prod_p = &product;
  
  printf("\nSize of product: %d\n", sizeof product);
  
  return 0;
}