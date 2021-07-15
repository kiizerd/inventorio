
const struct nk_vec2 POS_DEFAULT = {(float)25, (float)25};
const struct nk_vec2 POS_HIDDEN  = {(float)-WINDOW_WIDTH * 1.2, (float)25};

int has_been_init = 0;
int *init_p = &has_been_init;

static void show_window(struct nk_context *ctx, const char *name) {
  nk_window_set_position(ctx, name, POS_DEFAULT);
}

static void hide_window(struct nk_context *ctx, const char *name) {
  nk_window_set_position(ctx, name, POS_HIDDEN);
}

static int
init_new_inventory_window(struct nk_context *ctx)
{
  if (nk_begin(ctx, "New Inventory", nk_rect(10, 10, 350, 600),
      NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
  {
    static const float ratio[] = {80, 150};
    static char prop_buffer[9][25];
    static char desc_buffer[200];
    static int text_len[9];
    static int desc_len;

    nk_layout_row(ctx, NK_STATIC, 25, 2, ratio);
    nk_label(ctx, "Name:", NK_TEXT_LEFT);
    nk_edit_string(ctx, NK_EDIT_FIELD | NK_EDIT_SIG_ENTER, prop_buffer[0], &text_len[0], 42, nk_filter_default);
    nk_label(ctx, "Description:", NK_TEXT_LEFT);
    nk_layout_row_static(ctx, 120, 300, 1);
    nk_edit_string(ctx, NK_EDIT_BOX, desc_buffer, &desc_len, 200, nk_filter_default);

    nk_layout_row_static(ctx, 25, 300, 1);
    if (nk_button_label(ctx, "Create Product")) {
      1;
    }

  }
  nk_end(ctx);
  return !nk_window_is_closed(ctx, "New Inventory");
}

static int
init_new_product_window(struct nk_context *ctx)
{
  if (nk_begin(ctx, "New Product", nk_rect(25, 25, 350, 550),
               NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
                   NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE | NK_WINDOW_CLOSABLE))
  {
    static const float ratio[] = {80, 150};
    static char prop_buffer[9][25];
    static char desc_buffer[200];
    static int text_len[9];
    static int desc_len;

    nk_layout_row(ctx, NK_STATIC, 25, 2, ratio);
    nk_label(ctx, "Title:", NK_TEXT_LEFT);
    nk_edit_string(ctx, NK_EDIT_FIELD | NK_EDIT_SIG_ENTER, prop_buffer[0], &text_len[0], 42, nk_filter_default);
    nk_label(ctx, "Description:", NK_TEXT_LEFT);
    nk_layout_row_static(ctx, 120, 300, 1);
    nk_edit_string(ctx, NK_EDIT_BOX, desc_buffer, &desc_len, 200, nk_filter_default);

    nk_layout_row_static(ctx, 25, 300, 1);
    if (nk_button_label(ctx, "Create Product"))
      inv_create_product(&prop_buffer[0], &desc_buffer, NULL, 1.1, 1);
  }
  nk_end(ctx);
  return !nk_window_is_closed(ctx, "New Product");
}

static int
init_gui_windows(struct nk_context *ctx)
{
  init_new_inventory_window(ctx);
  init_new_product_window(ctx);
  if (!has_been_init) {
    hide_window(ctx, "New Inventory");
    hide_window(ctx, "New Product");
    has_been_init = 1;
  }

  return 0;
}