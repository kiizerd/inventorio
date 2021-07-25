// #include "lib/nuklear.h"

int windows_hidden = 0;
int *init_p = &windows_hidden;

#define INV_WND_WIDTH (float)WINDOW_WIDTH * 0.8
#define INV_WND_HEIGHT (float)WINDOW_HEIGHT * 0.8
#define INV_WND_OFFSET_X ((float)WINDOW_WIDTH / 2) - ((float)INV_WND_WIDTH / 2)
#define INV_WND_OFFSET_Y ((float)WINDOW_HEIGHT / 2) - ((float)INV_WND_HEIGHT * 1.8)
#define INV_WND_BTN_W 36.0F
#define INV_WND_BTN_H 36.0F
#define INV_WND_INT_W (float)INV_WND_WIDTH * 0.91
#define INV_WND_INT_H (float)INV_WND_HEIGHT * 0.84

static void
die(const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);
  fputs("\n", stderr);
  exit(EXIT_FAILURE);
}

static struct nk_image
icon_load(const char *filename)
{
  int x, y, n;
  GLuint tex;
  unsigned char *data = stbi_load(filename, &x, &y, &n, 0);
  if (!data)
    die("[SDL]: failed to load image: %s", filename);

  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);
  return nk_image_id((int)tex);
}

static struct inv_icons
init_inv_icons(struct nk_context *ctx)
{
  struct inv_icons icons;

  icons.server_host = icon_load("../assets/icons/server_host.png");
  icons.server_config = icon_load("../assets/icons/server_config.png");
  icons.server_close = icon_load("../assets/icons/server_close.png");
  icons.search_prods = icon_load("../assets/icons/search_prods.png");
  icons.view_tags = icon_load("../assets/icons/view_tags.png");
  icons.create_prod = icon_load("../assets/icons/create_prod.png");
  icons.view_prod = icon_load("../assets/icons/view_prod.png");
  icons.edit_prod = icon_load("../assets/icons/edit_prod.png");
  icons.delete_prod = icon_load("../assets/icons/delete_prod.png");

  ctx->icons = icons;
  return icons;
}

static int
init_main_menu_window(struct nk_context *ctx)
{
  /* Main Menu - Entry Point */
#define MMENU_BTN_WIDTH (int)WINDOW_WIDTH * 0.25
#define MMENU_PAD_WIDTH (int)MMENU_BTN_WIDTH * 0.08

  static const float ratio[] = {MMENU_PAD_WIDTH, MMENU_BTN_WIDTH, MMENU_PAD_WIDTH};

#define MMENU_WIDTH (float)(ratio[0] + ratio[1] + ratio[2] + 35)
#define MMENU_HEIGHT MMENU_WIDTH - 100

#define MMENU_OFFSET_X ((float)WINDOW_WIDTH / 2) - ((float)MMENU_WIDTH / 2)
#define MMENU_OFFSET_Y ((float)WINDOW_HEIGHT / 2) - ((float)MMENU_HEIGHT * 1.8)

  struct
      nk_rect inv_mmenu_nk_rect = nk_rect(MMENU_OFFSET_X, MMENU_OFFSET_Y, MMENU_WIDTH, MMENU_HEIGHT);
  nk_flags inv_mmenu_nk_flags = NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE;
  if (nk_begin(ctx, "Main Menu", inv_mmenu_nk_rect, inv_mmenu_nk_flags))
  {
    nk_layout_row(ctx, NK_STATIC, 40, 3, ratio);
    nk_label(ctx, "", NK_TEXT_LEFT);
    if (nk_button_label(ctx, "Open Inventory"))
    {
      inv_open_inv_from_file();
    }
    nk_label(ctx, "", NK_TEXT_RIGHT);

    nk_label(ctx, "", NK_TEXT_LEFT);
    if (nk_button_label(ctx, "Connect to Inventory"))
    {
    }
    nk_label(ctx, "", NK_TEXT_RIGHT);

    nk_label(ctx, "", NK_TEXT_LEFT);
    if (nk_button_label(ctx, "New Inventory"))
    {
      nk_show_window(ctx, "New Inventory");
    }
    nk_label(ctx, "", NK_TEXT_RIGHT);

    nk_label(ctx, "", NK_TEXT_LEFT);
    if (nk_button_label(ctx, "Settings"))
    {
    }
    nk_label(ctx, "", NK_TEXT_RIGHT);
  }
  nk_end(ctx);
  return !nk_window_is_closed(ctx, "Main Menu");
}

static int
fill_inv_view_pdt_list(struct nk_context *ctx, struct inv_inventory *inv)
{

  return 0;
}

static int
init_inventory_view_window(struct nk_context *ctx, struct inv_inventory *inv)
{
  struct nk_image server_host = ctx->icons.server_host;
  struct nk_image server_config = ctx->icons.server_config;
  struct nk_image server_close = ctx->icons.server_close;
  struct nk_image view_tags = ctx->icons.view_tags;
  struct nk_image search_prods = ctx->icons.search_prods;
  struct nk_image create_prod = ctx->icons.create_prod;
  struct nk_image view_prod = ctx->icons.view_tags;
  struct nk_image edit_prod = ctx->icons.edit_prod;
  struct nk_image delete_prod = ctx->icons.delete_prod;

  struct
      nk_rect inv_wnd_nk_rect = nk_rect(INV_WND_OFFSET_X, INV_WND_OFFSET_Y, INV_WND_WIDTH, INV_WND_HEIGHT);
  nk_flags inv_wnd_nk_flags = NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE;
  if (nk_begin(ctx, "Inventory View", inv_wnd_nk_rect, inv_wnd_nk_flags))
  {
    /* Toolbar */

    /* Button styling */
    {
      struct nk_vec2 btn_pad = {0.0F, 0.0F};
      struct nk_vec2 btn_rnd = {0.0F, 0.0F};
      struct nk_vec2 btn_bdr = {0.0F, 0.0F};

      nk_style_push_vec2(ctx, &ctx->style.button.image_padding, btn_pad);
      nk_style_push_vec2(ctx, &ctx->style.button.touch_padding, btn_pad);
      nk_style_push_vec2(ctx, &ctx->style.button.padding, btn_pad);

      nk_style_push_vec2(ctx, &ctx->style.button.rounding, btn_rnd);
      nk_style_push_vec2(ctx, &ctx->style.button.border, btn_bdr);
    }

    float btn_width = INV_WND_BTN_W;
    float menu_height = 38.0F;

    float t_bar_left_margin = 5.0F;

    nk_menubar_begin(ctx);
    nk_layout_row_begin(ctx, NK_STATIC, menu_height, 7);

    nk_layout_row_push(ctx, t_bar_left_margin); // Left margin
    nk_spacer_simple(ctx);

    /* Server buttons */
    {
      nk_layout_row_push(ctx, btn_width); // Host inventory server button
      if (nk_button_image(ctx, server_host))
      {
        // TODO - figure out how to host a server lmao
      }
      nk_layout_row_push(ctx, btn_width); // Config hosted server button
      if (nk_button_image(ctx, server_config))
      {
        // TODO - have settings to configure
      }
      nk_layout_row_push(ctx, btn_width); // Close hosted server button
      if (nk_button_image(ctx, server_close))
      {
        // TODO - close theoretical server
      }
    }

    /* Middle spacer */
    float mid_spacer_w = (float)(INV_WND_WIDTH * 0.5F) - (INV_WND_WIDTH * 0.16F);
    nk_layout_row_push(ctx, mid_spacer_w);
    nk_spacer_simple(ctx);

    nk_layout_row_push(ctx, btn_width); // View tags button
    if (nk_button_image(ctx, view_tags))
    {
      1;
    }

    nk_layout_row_push(ctx, btn_width); // Search products button
    if (nk_button_image(ctx, search_prods))
    {
      1;
    }

    nk_menubar_end(ctx);

    /* Main view */
    // contains product table and
    // sidebar with product CRUD buttons
    float main_view_h = (float)INV_WND_HEIGHT * 0.8;
    float table_width = (float)INV_WND_WIDTH * 0.91;
    float s_bar_width = (float)INV_WND_WIDTH * 0.05;

    const float main_view_ratio[] = {
        table_width,
        s_bar_width};

    nk_layout_row(ctx, NK_STATIC, main_view_h, 2, main_view_ratio);

    /* Product Table */
    nk_flags table_flags = NK_WINDOW_BORDER;
    nk_flags s_bar_flags = 0;
    if (nk_group_begin(ctx, "Product table", table_flags))
    {
      const float col_ratio[6] = {
          (float)INV_WND_WIDTH * 0.19F /* 19% */,
          (float)INV_WND_WIDTH * 0.23F /* 23% */,
          (float)INV_WND_WIDTH * 0.09F /*  9% */,
          (float)INV_WND_WIDTH * 0.09F /*  9% */,
          (float)INV_WND_WIDTH * 0.11F /* 11% */,
          (float)INV_WND_WIDTH * 0.14F /* 14% */
      };

      /* Column headers */
      {
        int sort_by[5];

        nk_layout_row(ctx, NK_STATIC, 16, 6, col_ratio);
        if (nk_selectable_label(ctx, "Title", NK_TEXT_ALIGN_CENTERED, &sort_by[0]))
        {
          1;
        }
        if (nk_selectable_label(ctx, "Tags", NK_TEXT_ALIGN_CENTERED, &sort_by[1]))
        {
          1;
        }
        if (nk_selectable_label(ctx, "Price", NK_TEXT_ALIGN_CENTERED, &sort_by[2]))
        {
          1;
        }
        if (nk_selectable_label(ctx, "Quantity", NK_TEXT_ALIGN_CENTERED, &sort_by[3]))
        {
          1;
        }
        if (nk_selectable_label(ctx, "ID", NK_TEXT_ALIGN_CENTERED, &sort_by[4]))
        {
          1;
        }
        int empty = 0;
        if (nk_selectable_label(ctx, "Desc.", NK_TEXT_ALIGN_CENTERED, &empty))
        {
          1;
        }
      }

      /* Product List */
      {
        if (inv)
        {
          int count = (int)inv->pdt_count;

          int i;
          for (i = 0; i < count; i++)
          {
            struct inv_product *current = *&inv->products[i];

            nk_label(ctx, current->title, NK_TEXT_ALIGN_LEFT);
            nk_label(ctx, current->tags[0], NK_TEXT_ALIGN_LEFT);
            nk_label(ctx, current->price, NK_TEXT_ALIGN_LEFT);
            nk_label(ctx, current->quantity, NK_TEXT_ALIGN_LEFT);
            nk_label(ctx, current->id, NK_TEXT_ALIGN_LEFT);
            nk_label(ctx, current->description, NK_TEXT_ALIGN_LEFT);
          }
        }
      }

      nk_group_end(ctx);
    }

    /* Product CRUD sidebar */
    /* Sidebar styles */
    {
      struct nk_vec2 s_bar_pad = {0.0F, 0.0F};
      // struct nk_vec2 s_bar_brd = {0.0F, 0.0F};
      struct nk_color s_bar_color = nk_rgba(30, 133, 40, 215);
      nk_style_push_vec2(ctx, &ctx->style.window.group_padding, s_bar_pad);
      // nk_style_push_vec2(ctx, &ctx->style.window.group_border, s_bar_brd);
      nk_style_push_color(ctx, &ctx->style.window.group_border_color, s_bar_color);
    }

    if (nk_group_begin(ctx, "Sidebar", s_bar_flags))
    {
      nk_layout_row_static(ctx, btn_width, btn_width, 1); // Create product button
      if (nk_button_image(ctx, create_prod))
      {
        nk_show_window(ctx, "New Product");
      }
      if (nk_button_image(ctx, view_prod))
      {
        1;
      }
      if (nk_button_image(ctx, edit_prod))
      {
        1;
      }
      if (nk_button_image(ctx, delete_prod))
      {
        1;
      }

      nk_group_end(ctx);
    }

    nk_style_pop_color(ctx);
    // nk_style_pop_vec2(ctx);
    nk_style_pop_vec2(ctx);
    nk_style_pop_vec2(ctx);
    nk_style_pop_vec2(ctx);
    nk_style_pop_vec2(ctx);
    nk_style_pop_vec2(ctx);
    nk_style_pop_vec2(ctx);
  }
  nk_end(ctx);
}

static int
init_new_inventory_window(struct nk_context *ctx)
{
  nk_flags new_inv_flags = NK_WINDOW_BORDER | NK_WINDOW_MOVABLE |
                           NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE;
  if (nk_begin(ctx, "New Inventory", nk_rect(10, 10, 270, 400), new_inv_flags))
  {
    const float ratio[] = {80, 150};
    const float ratio2[] = {130, 100};

    static char name_buffer[TITLE_MAX];
    static int name_len;

    static t_ctg tags_buffer;
    static t_ctg *tags_buffer_p = &tags_buffer;
    static char temp_tag_buffer[TAGS_LEN];
    static int tags_len;
    static int tag_count = 0;

    nk_layout_row(ctx, NK_STATIC, 25, 2, ratio);
    nk_label(ctx, "Name:", NK_TEXT_LEFT);
    nk_edit_string(ctx, NK_EDIT_SIMPLE, name_buffer, &name_len, TITLE_MAX, nk_filter_default);

    // TODO - extract tag group to function

    nk_layout_row_static(ctx, 15, 225, 1); // spacer row

    static int group_width = 230;
    static int group_height = 200;

    nk_flags group_flags = NK_WINDOW_BORDER | NK_WINDOW_TITLE;

    nk_layout_row_static(ctx, (float)group_height, group_width, 2);
    if (nk_group_begin(ctx, "Tags:", group_flags))
    {
      int i;
      static int selected[TAGS_MAX];

      nk_layout_row_static(ctx, 15, 100, 1);
      for (i = 0; i < tag_count; i++)
      {
        struct nk_rect bounds = nk_widget_bounds(ctx);
        nk_selectable_label(ctx, tags_buffer[i], NK_TEXT_ALIGN_LEFT, &selected[i]);
        if (nk_contextual_begin(ctx, 0, nk_vec2(110, 300), bounds))
        {
          nk_layout_row_static(ctx, 15, 90, 1);
          if (nk_button_image_label(ctx, ctx->icons.delete_prod, "Delete", NK_TEXT_ALIGN_CENTERED))
          {
            // TODO - delete_tag() here
          }
          nk_contextual_end(ctx);
        }
      }
      nk_group_end(ctx);
    }

    nk_flags active;
    nk_flags tag_field_flags = NK_EDIT_FIELD | NK_EDIT_SIG_ENTER;
    int i = 0;
    nk_layout_row(ctx, NK_STATIC, 25, 2, ratio2);
    active = nk_edit_string(ctx, tag_field_flags, &temp_tag_buffer, &tags_len, TAGS_LEN, nk_filter_ascii);
    if (nk_button_label(ctx, "Add tag") || (active & NK_EDIT_COMMITED))
    {
      if (tags_len != 0)
      {
        memcpy(tags_buffer[tag_count], &temp_tag_buffer, (nk_size)tags_len);
        tags_len = 0;
        tag_count++;
      }
    }

    nk_layout_row_static(ctx, 15, 230, 1); // spacer row

    nk_layout_row_static(ctx, 25, 233, 1);
    if (nk_button_label(ctx, "Create and open inventory"))
    {
      if (nk_strlen(name_buffer) != 0)
      {
        inv_create_inventory(&name_buffer, tags_buffer, tag_count);

        nk_hide_window(ctx, "New Inventory");
        nk_show_window(ctx, "Inventory View");

        // Reset form when inventory created
        int i;
        for (i = 0; i < tag_count; i++)
        {
          tags_len = 0;
        }

        tag_count = 0;
        name_len = 0;
      }
    }
  }
  nk_end(ctx);
  return !nk_window_is_closed(ctx, "New Inventory");
}

static int
init_new_product_window(struct nk_context *ctx)
{
  nk_flags new_pdt_flags = NK_WINDOW_BORDER | NK_WINDOW_MOVABLE |
                           NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE;

  if (nk_begin(ctx, "New Product", nk_rect(150.0F, 150.0F, 270.0F, 650.0F), new_pdt_flags))
  {
    static const float ratio[] = {80, 150};

    static char props[4][DESC_MAX];
    static int prop_len[4];

    nk_layout_row(ctx, NK_STATIC, 25, 2, ratio);
    nk_label(ctx, "Title:", NK_TEXT_LEFT);
    nk_edit_string(ctx, NK_EDIT_FIELD, props[0], &prop_len[0], TITLE_MAX, nk_filter_default);
    nk_label(ctx, "Price:", NK_TEXT_LEFT);
    nk_edit_string(ctx, NK_EDIT_FIELD, props[1], &prop_len[1], 12, nk_filter_float);
    nk_label(ctx, "Quantity:", NK_TEXT_LEFT);
    nk_edit_string(ctx, NK_EDIT_FIELD, props[2], &prop_len[2], 12, nk_filter_decimal);
    nk_label(ctx, "Description:", NK_TEXT_LEFT);
    nk_layout_row_static(ctx, 120, 233, 1);
    nk_edit_string(ctx, NK_EDIT_BOX, props[3], &prop_len[3], DESC_MAX, nk_filter_default);

    // TODO - extract tag group to function

    const float ratio2[] = {80, 150};
    const float ratio3[] = {130, 100};

    static t_ctg tags_buffer;
    static t_ctg *tags_buffer_p = &tags_buffer;
    static char temp_tag_buffer[TAGS_LEN];
    static int tags_len;
    static int tag_count;

    nk_layout_row_static(ctx, 15, 225, 1); // spacer row

    static int group_width = 230;
    static int group_height = 200;

    nk_flags group_flags = NK_WINDOW_BORDER | NK_WINDOW_TITLE;

    nk_layout_row_static(ctx, (float)group_height, group_width, 2);
    if (nk_group_begin(ctx, "Tags:", group_flags))
    {
      int i;
      static int selected[TAGS_MAX];

      nk_layout_row_static(ctx, 15, 100, 1);
      for (i = 0; i < tag_count; i++)
      {
        struct nk_rect bounds = nk_widget_bounds(ctx);
        nk_selectable_label(ctx, tags_buffer[i], NK_TEXT_ALIGN_LEFT, &selected[i]);
        if (nk_contextual_begin(ctx, 0, nk_vec2(110, 300), bounds))
        {
          nk_layout_row_static(ctx, 15, 90, 1);
          if (nk_button_image_label(ctx, ctx->icons.delete_prod, "Delete", NK_TEXT_ALIGN_CENTERED))
          {
            // TODO - Add delete_tag() here
          }
          nk_contextual_end(ctx);
        }
      }
      nk_group_end(ctx);
    }

    nk_flags active;
    nk_flags tag_field_flags = NK_EDIT_FIELD | NK_EDIT_SIG_ENTER;
    int i = 0;
    nk_layout_row(ctx, NK_STATIC, 25, 2, ratio3);
    active = nk_edit_string(ctx, tag_field_flags, &temp_tag_buffer, &tags_len, TAGS_LEN, nk_filter_ascii);
    if (nk_button_label(ctx, "Add tag") || (active & NK_EDIT_COMMITED))
    {
      if (tags_len != 0)
      {
        memcpy(tags_buffer[tag_count], &temp_tag_buffer, (nk_size)tags_len);
        tags_len = 0;
        tag_count++;
      }
    }

    nk_layout_row_static(ctx, 25, 233, 1);
    if (nk_button_label(ctx, "Create Product"))
    {

      // TODO - validate form where needed

      inv_create_product(props[0], props[1], props[2], props[3], tags_buffer, tag_count);
      nk_hide_window(ctx, "New Product");

      // Reset form after creation
      int i;
      for (i = 0; i < 4; i++) {
        prop_len[i] = 0;
        tags_len = 0;
      }
      tag_count = 0;
    }
  }
  nk_end(ctx);
  return !nk_window_is_closed(ctx, "New Product");
}

static int
init_gui_windows(struct nk_context *ctx)
{
  init_main_menu_window(ctx);
  init_new_inventory_window(ctx);
  init_new_product_window(ctx);
  init_inventory_view_window(ctx, active_inv);
  if (!windows_hidden)
  {
    nk_hide_window(ctx, "New Inventory");
    nk_hide_window(ctx, "New Product");
    nk_hide_window(ctx, "Inventory View");
    windows_hidden = 1;
  }

  return 0;
}