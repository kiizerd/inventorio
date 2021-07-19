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

typedef char PDTID[ID_LEN];


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
  char    title[TITLE_MAX];
  char    description[DESC_MAX];
  float   price;
  ushort  quantity;
  ushort  numSold;
  p_t_ctg tags;
  ushort  tag_count;
  PDTID   id;
};

typedef struct inv_product inv_pdt;

struct pdt_catalog {
  inv_pdt *products[PRODUCTS_MAX];
  PDTID   pdt_ids[PRODUCTS_MAX];
  ushort  pdt_qnt[PRODUCTS_MAX];
  ushort  count;
};

typedef struct pdt_catalog p_ctg;

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
  char    name[TITLE_MAX];
  inv_pdt products[PRODUCTS_MAX];
  p_ctg   catalog;
  ushort  pdt_count;
  t_ctg   tags;
  ushort  tag_count;
};


// const static struct inv_inventory *inv_catalog[];

INV_API char * inv_generate_pdtid
(struct inv_inventory *inv_p, char *title)
{
  int id_check = 1;
  char *id_str_p, id_string[ID_LEN];
  struct inv_inventory inv = *inv_p;
  do {
    int i, j;
    id_str_p = id_string;

    id_string[0] = title[0];
    id_string[1] = title[2];

    for (i = 0; i < ID_LEN - 1; i++) {
      id_string[i + 2] = (rand() % 9) + '0';
    }

    int count = (int)&inv.catalog.count;
    for (j = 0; j < count; j++) {
      if (strcmp(id_string, inv.catalog.pdt_ids[j]) == 0) {
        id_check = 0;
        printf("generated id matches existing id, regenerating");
      }
    }
  
  } while (id_check);

  return id_str_p;
}

// All functions below this point will perform an operation on an inventory struct or its constituents

static struct inv_inventory *active_inv;

INV_API void inv_add_product_to_inv
(struct inv_inventory *inv_p, struct inv_product *pdt_p)
{  
  struct inv_inventory inv = *inv_p;
  struct inv_product pdt   = *pdt_p;
  memcpy(&inv.products[inv.pdt_count], pdt_p, sizeof(*pdt_p));
  memcpy(&inv.catalog.pdt_ids[inv.pdt_count++], &pdt.id, sizeof pdt.id);
  inv.catalog.pdt_qnt[inv.pdt_count] = pdt.quantity;

  inv.pdt_count++;

  int i;
  for (i = 0; i < inv.pdt_count; i++) {
    printf("Product title: %s\nProduct description: %s\n", inv.products[i].title, inv.products[i].description);
  }
}

INV_API void inv_create_product
(char *title, char *desc, struct pdt_tags *tags_p, float price, int quantity)
{  
  struct inv_inventory inv = *active_inv;
  struct inv_product pdt;
  
  strcpy(pdt.title, title ? title : "new_product");

  strcpy(pdt.description, desc ? desc : "n/a");

  PDTID *new_pdt_id = inv_generate_pdtid(&inv, pdt.title);
  strcpy(pdt.id, new_pdt_id);

  // TODO - loop thru tags and strcpy all
  if (tags_p) {
    1;
  }

  pdt.quantity = quantity ? quantity : 0;
  pdt.price    = price    ? price    : 5.0F;
  pdt.numSold  = 0;

  inv_add_product_to_inv(&inv, &pdt);
}

// TODO - write created inventory to disk/database
INV_API void inv_create_inventory
(struct nk_context *ctx, char *title, char tags[TAGS_MAX][TAGS_LEN], int tag_count)
{
  struct inv_inventory new_inv;

  strcpy(new_inv.name, title);

  int i;
  for (i = 0; i < tag_count; i++) {
    strcpy(new_inv.tags[i], tags[i]);
  }

  new_inv.tag_count = tag_count;

  active_inv = &new_inv;

  nk_hide_window(ctx, "New Inventory");

  // write inventory to disk/database
}

INV_API void inv_open_inv_from_file()
{
#ifdef _WIN32
      open_file_browser(wnd);
#elif defined __linux__
      int doplhin = system("dolphin --new-window");
#endif
}