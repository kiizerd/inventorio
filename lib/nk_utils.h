const struct nk_vec2 POS_DEFAULT = {(float)25, (float)25};
const struct nk_vec2 POS_HIDDEN  = {(float)-WINDOW_WIDTH * 1.2, (float)25};

static void nk_show_window(struct nk_context *ctx, const char *name) {
  nk_window_set_position(ctx, name, POS_DEFAULT);
}

static void nk_hide_window(struct nk_context *ctx, const char *name) {
  nk_window_set_position(ctx, name, POS_HIDDEN);
}
