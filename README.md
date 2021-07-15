 ### Inventorio - Inventory manager
 #  

 #### Summary
  A (hopefully) simple and lightweight inventory management app. Intended to be cross-platform (Windows, macOS, Linux).
  Very much WIP.

  When finished I'd like this to be able to handle most standard operations that would be expected.
    - A database of CRUD-able items (products in this case)
    - A list of CRUD-able categories to organize items
    - sorting and searching by all product properties

 #### Outline

 - Master Inventory object that contains many products
 - CRUD inventories
 - CRUD operations for products.
 - CRUD operations for product-categories.
 - Product Catalog
 - Simple GUI
 - Purchaseable items

 - Multiple clients(registers) can connect to 1 server(inventory)

 ---

 ### TODO
 # 
 - UI  
 <-- Powered by Nukleus -->
  - Entry window
    - simple start menu
    - has buttons for:
      - new inv.
      - load inv.
      - connect to inv.
      - configure settings

  - Product View window
    - shows detailed info for product
    - has buttons for:
      - clone
      - edit
      - delete
      - buy/sell
      - hide

  - Add Product window
    - simple form

  - Tag view window
    - list all created tags
      - selectable items
    - buttons for:
      - create tag
      - delete tag        (selected|right-click)
      - edit tag          (selected|right-click)
      - find all with tag (selected|right-click)

  - Inventory View window
    - shows all products and inv. details
    - has buttons for:
      - host inv.
      - configure inv. settings
      - add product
      - show tag view window
    - nice looking, clean, product list
      - product right click menu
      - double click product menu shows product view window
      - sort and search products by:
       - title
       - description
       - ID
       - tags

  - Add inventory window
    - simple form

  - Register window

 # 
