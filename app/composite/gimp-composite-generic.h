#ifndef gimp_composite_generic_h
#define gimp_composite_generic_h

extern gboolean gimp_composite_generic_init (void);
/*
	* The function gimp_composite_*_install() is defined in the code generated by make-install.py
	* I hate to create a .h file just for that declaration, so I do it here (for now).
 */
extern gboolean gimp_composite_generic_install (void);

void gimp_composite_addition_any_any_any_generic (GimpCompositeContext *);
void gimp_composite_anti_erase_any_any_any_generic (GimpCompositeContext *);
void gimp_composite_blend_any_any_any_generic (GimpCompositeContext *);
void gimp_composite_burn_any_any_any_generic (GimpCompositeContext *);
void gimp_composite_color_any_any_any_generic (GimpCompositeContext *);
void gimp_composite_color_erase_any_any_any_generic (GimpCompositeContext *);
void gimp_composite_color_only_any_any_any_generic (GimpCompositeContext *);
void gimp_composite_convert_any_any_any_generic (GimpCompositeContext *);
void gimp_composite_darken_any_any_any_generic (GimpCompositeContext *);
void gimp_composite_difference_any_any_any_generic (GimpCompositeContext *);
void gimp_composite_dissolve_any_any_any_generic (GimpCompositeContext *);
void gimp_composite_divide_any_any_any_generic (GimpCompositeContext *);
void gimp_composite_dodge_any_any_any_generic (GimpCompositeContext *);
void gimp_composite_erase_any_any_any_generic (GimpCompositeContext *);
void gimp_composite_grain_extract_any_any_any_generic (GimpCompositeContext *);
void gimp_composite_grain_merge_any_any_any_generic (GimpCompositeContext *);
void gimp_composite_hardlight_any_any_any_generic (GimpCompositeContext *);
void gimp_composite_hue_any_any_any_generic (GimpCompositeContext *);
void gimp_composite_lighten_any_any_any_generic (GimpCompositeContext *);
void gimp_composite_multiply_any_any_any_generic (GimpCompositeContext *);
void gimp_composite_normal_any_any_any_generic (GimpCompositeContext *);
void gimp_composite_overlay_any_any_any_generic (GimpCompositeContext *);
void gimp_composite_replace_any_any_any_generic (GimpCompositeContext *);
void gimp_composite_saturation_any_any_any_generic (GimpCompositeContext *);
void gimp_composite_scale_any_any_any_generic (GimpCompositeContext *);
void gimp_composite_screen_any_any_any_generic (GimpCompositeContext *);
void gimp_composite_softlight_any_any_any_generic (GimpCompositeContext *);
void gimp_composite_subtract_any_any_any_generic (GimpCompositeContext *);
void gimp_composite_swap_any_any_any_generic (GimpCompositeContext *);
void gimp_composite_value_any_any_any_generic (GimpCompositeContext *);
void gimp_composite_behind_any_any_any_generic (GimpCompositeContext * );
#endif
