const struct nk_vec2 POS_DEFAULT = {(float)25, (float)25};
const struct nk_vec2 POS_HIDDEN  = {(float)-WINDOW_WIDTH * 1.2, (float)25};

static void
nk_show_window(struct nk_context *ctx, const char *name)
{
  nk_window_set_position(ctx, name, POS_DEFAULT);
  nk_window_set_focus(ctx, name);
}

static void 
nk_hide_window(struct nk_context *ctx, const char *name)
{
  nk_window_set_position(ctx, name, POS_HIDDEN);
}

/*#### nk_spacer_simple
/// Creates an empty space of default size */
static void
nk_spacer_simple(struct nk_context *ctx)
{
  struct nk_rect dimensions = {};

  nk_panel_alloc_space(&dimensions, ctx);
}

/* #### nk_spacer_sized_width
/// Creates and empty space of given width and default height */
static void
nk_spacer_sized_width(struct nk_context *ctx, float width)
{
  struct nk_rect dimensions;
  dimensions = nk_rect(0.0F, 0.0F, (float)width, 0.0F);

  nk_panel_alloc_space(&dimensions, ctx);
}

/* #### nk_spacer_sized_height
/// Creates and empty space of given height and default width */
static void
nk_spacer_sized_height(struct nk_context *ctx, float height)
{
  struct nk_rect dimensions;
  dimensions = nk_rect(0.0F, 0.0F, 0.0F, (float)height);

  nk_panel_alloc_space(&dimensions, ctx);
}

/* #### nk_spacer_sized
/// Creates and empty space of given width and height */
static void
nk_spacer_sized(struct nk_context *ctx, float width, float height)
{
  struct nk_rect dimensions;
  dimensions = nk_rect(0.0F, 0.0F, (float)width, (float)height);

  nk_panel_alloc_space(&dimensions, ctx);
}

/* #### nk_spacer
/// Creates an empty space for layouting (Width X Height)
/// 
/// width -- optional - if NULL or 0, creates spacer of default width
/// height - optional - if NULL or 0, creates spacer of default height
/// 
/// Expands to 
///  nk_spacer_simple(...)
///   - if width and height omitted
/// 
///  nk_spacer_sized_width(...)
///   - if width included but height omitted
/// 
///  nk_spacer_sized_height(...) 
///   - if height included but width omitted
/// 
///  nk_spacer_sized(...) 
///   - if width and height included */
static void
nk_spacer(struct nk_context *ctx, float width, float height)
{
  if ((!width) && (!height)){
    nk_spacer_simple(ctx);
  } else if ((width) && (!height)) {
    nk_spacer_sized_width(ctx, width);
  } else if ((height) && (!width)) {
    nk_spacer_sized_height(ctx, height);
  } else {
    nk_spacer_sizer(ctx, width, height);    
  }
  
}
