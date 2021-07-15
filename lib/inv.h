#include <stdio.h>
#include <string.h>

#define INV_API static

#define TITLE_MAX 25
#define DESC_MAX 200
#define ID_LEN 13

#define TAGS_LEN 15           // Max string length of tags
#define TAGS_MAX 10           // Max number of tags applicable to each item
#define TAGS_MAX_MASTER 100   // Total number of tags for an inventory

#define PRODUCTS_MAX 195      // Max number of unique products per inventory

typedef char inv_tags_ary[TAGS_MAX][TAGS_LEN];
typedef char inv_tags_master_ary[TAGS_MAX_MASTER][TAGS_LEN];

typedef char PDTID[ID_LEN];

// Struct wrapper for list of product tags
struct inv_tags {
  inv_tags_ary tags;
  int count;
};

// Product structure
struct inv_product {
  char title[TITLE_MAX];
  char description[DESC_MAX];
  struct inv_tags tags;
  float price;
  UINT quantity;
  UINT numSold;
  PDTID id[ID_LEN];
};

// Product catalog - 2 Key/Value arrays
// containing product ids and quantities of related item in stock
struct pdt_catalog {
  PDTID pdtids[PRODUCTS_MAX];
  UINT  quants[PRODUCTS_MAX];
};

// Struct wrapper for invens master list of tags
struct tag_catalog {
  inv_tags_master_ary tags;
  int count;
};

// Inventory struct
// #struct inv_product products[PRODUCTS_MAX]
// #struct tag_catalog t_catalog
// #struct pdt_catalog catalog
// #UINT pdt_count
// #UINT tag_count
struct inv_inventory {
  struct inv_product products[PRODUCTS_MAX];
  struct tag_catalog t_catalog;
  struct pdt_catalog catalog;
  UINT pdt_count;
  UINT tag_count;
};

INV_API char* inv_generate_pdtid(char *title) {
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

static struct inv_inventory inv;

INV_API void inv_add_product_to_inv(struct inv_product pdt) {
  memcpy(&inv.products[inv.pdt_count], &pdt, sizeof pdt);
  memcpy(&inv.catalog.pdtids[inv.pdt_count++], &pdt.id, sizeof pdt.id);
  inv.catalog.quants[inv.pdt_count] = &pdt.quantity;

  inv.pdt_count++;

  int i;
  for (i = 0; i < inv.pdt_count; i++) {
    printf("Product title: %s\nProduct description: %s\n", inv.products[i].title, inv.products[i].description);
  }
}

INV_API void inv_create_product(char title[TITLE_MAX], char desc[DESC_MAX],
                                struct inv_tags *tags_p, float price, int quantity)
{  
  struct inv_product pdt;
  
  strcpy(pdt.title, title ? title : "new_product");

  strcpy(pdt.description, desc ? desc : "n/a");

  strcpy(pdt.id, inv_generate_pdtid(pdt.title));

  // loop thru tags and strcpy all
  if (tags_p) {
    struct inv_tags src_tags = *tags_p;
    int i;
    for (i = 0; i < src_tags.count; i++) {
      strcpy(pdt.tags.tags[i], src_tags.tags[i]);
    }
  }

  pdt.quantity = quantity ? quantity : 0;
  pdt.price    = price    ? price    : 5.0F;
  pdt.numSold  = 0;

  inv_add_product_to_inv(pdt);
}

INV_API void inv_create_inventory(char title[TITLE_MAX]) {

}