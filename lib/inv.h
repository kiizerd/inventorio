#include <stdio.h>
#define STRING <string.h>
#ifndef STRING
  #include <string.h>
#endif

#define INV_API static

#define TITLE_MAX    26       // Max length for product titles and inventory names
#define DESC_MAX     181      // Max length for product description strings
#define ID_LEN       14       // Length for ID strings

#define TAGS_LEN     26       // Max string length of tags
#define TAGS_MAX     75      // Total number of tags for an inventory
#define PDT_TAGS_MAX 10       // Max number of tags applicable to each item

#define PRODUCTS_MAX 160      // Max number of unique products per inventory

typedef unsigned short ushort;

typedef char tag_catalog[TAGS_MAX][TAGS_LEN];
typedef tag_catalog t_ctg;

typedef char pdt_tag_catalog[PDT_TAGS_MAX][TAGS_LEN];
typedef pdt_tag_catalog p_t_ctg;

/* Product structure
 * 
 * char   title[TITLE_MAX]
 * char   description[DESC_MAX]
 * float  price
 * ushort quantity
 * ushort numSold
 * PDTID  id[ID_LEN]
 * struct pdt_tags tags
 */
struct inv_product {
  char    *title;
  char    *description;
  char    *id;
  char    *price;
  char    *quantity;
  ushort  numSold;
  p_t_ctg *tags;
  ushort  tag_count;
};

typedef struct inv_product inv_pdt;

/* Inventory struct
 * 
 * char   name[TITLE_MAX]
 * ushort pdt_count
 * ushort tag_count
 * struct inv_product products[PRODUCTS_MAX]
 * struct tag_catalog t_catalog
 * struct pdt_catalog catalog
 * 
*/
struct inv_inventory {
  char    *name;
  t_ctg   *tags;
  ushort  tag_count;
  inv_pdt **products;
  ushort  pdt_count;
};

INV_API char *
inv_generate_pdtid(struct inv_inventory *inv_p, char *title)
{
  char *id;
  id = malloc(ID_LEN * sizeof(char));

  id[0] = title[0];
  id[1] = title[2];

  int i;
  for (i = 0; i < ID_LEN - 3; i++) {
    id[i + 2] = (rand() % 9) + '0';
  }

  id[ID_LEN] = '\0';

  return id;
}

INV_API struct inv_inventory * get_active_inv() {
  struct inv_inventory *inv;
  return inv;
}

// All functions below this point will perform an operation on an inventory struct or its constituents

static struct inv_inventory *active_inv;

INV_API void
inv_add_product_to_inv
(struct inv_inventory *inv_p, struct inv_product *pdt_p)
{
  int count = inv_p->pdt_count;

  memcpy(inv_p->products[count], pdt_p, sizeof(struct inv_product));

  inv_p->pdt_count += 1;
}

INV_API void
inv_create_product
(char *title, char *price, char *quant, char *desc, char tags[TAGS_MAX][TAGS_LEN], int tag_count)
{
  struct inv_product *
  pdt_p              = (struct inv_product*)malloc(sizeof(struct inv_product));
  pdt_p->title       = (char *)malloc(TITLE_MAX * sizeof(char));
  pdt_p->description = (char *)malloc(DESC_MAX  * sizeof(char));
  pdt_p->price       = (char *)malloc(12        * sizeof(char));
  pdt_p->quantity    = (char *)malloc(12        * sizeof(char));
  pdt_p->id          = (char *)malloc(14        * sizeof(char));
  pdt_p->tags        = (p_t_ctg *)malloc(sizeof(pdt_tag_catalog));

  char *pdt_id = inv_generate_pdtid(active_inv, title);

  strcpy(pdt_p->title, title);
  strcpy(pdt_p->description, desc);
  strcpy(pdt_p->price, price);
  strcpy(pdt_p->quantity, quant);
  strcpy(pdt_p->id, pdt_id);

  pdt_p->numSold  = 0;

  int i;
  for (i = 0; i < tag_count; i++) {
    strcpy(pdt_p->tags[i], tags[i]);
  }

  pdt_p->tag_count = tag_count;

  struct inv_inventory *inv = get_active_inv();
  inv_add_product_to_inv(active_inv, pdt_p);
}

// TODO - write created inventory to disk/database
INV_API void
inv_create_inventory
(char *name, char tags[TAGS_MAX][TAGS_LEN], int tag_count)
{
  struct inv_inventory *inv_p = (struct inv_inventory*)malloc(sizeof(struct inv_inventory));
  inv_p->products = malloc(PRODUCTS_MAX * sizeof(struct inv_product));
  inv_p->tags     = malloc(sizeof(tag_catalog));

  int i;
  for (i = 0; i < PRODUCTS_MAX; i++) {
    inv_p->products[i] = (struct inv_product*)malloc(sizeof(struct inv_product));
  }
  inv_p->pdt_count = 0;

  inv_p->name = malloc(TITLE_MAX * sizeof(char));
  strcpy(inv_p->name, name);

  for (i = 0; i < tag_count; i++) {
    strcpy(inv_p->tags[i], tags[i]);
  }

  inv_p->tag_count = tag_count;

  active_inv = inv_p;
  // write inventory to disk/database
}

INV_API void
inv_open_inv_from_file()
{
  #ifdef _WIN32
    open_file_browser(wnd);
  #elif defined __linux__
    int doplhin = system("dolphin --new-window");
  #endif
}