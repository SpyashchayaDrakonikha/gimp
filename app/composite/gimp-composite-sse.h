#ifndef gimp_composite_sse_h
#define gimp_composite_sse_h

extern gboolean gimp_composite_sse_init (void);

/*
	* The function gimp_composite_*_install() is defined in the code generated by make-install.py
	* I hate to create a .h file just for that declaration, so I do it here (for now).
 */
extern gboolean gimp_composite_sse_install (void);

#ifdef USE_MMX
#ifdef ARCH_X86
#if __GNUC__ >= 3
/*
 *
 */
extern void gimp_composite_addition_rgba8_rgba8_rgba8_sse (GimpCompositeContext *);
extern void gimp_composite_burn_rgba8_rgba8_rgba8_sse (GimpCompositeContext *);
extern void gimp_composite_coloronly_rgba8_rgba8_rgba8_sse (GimpCompositeContext *);
extern void gimp_composite_darken_rgba8_rgba8_rgba8_sse (GimpCompositeContext *);
extern void gimp_composite_difference_rgba8_rgba8_rgba8_sse (GimpCompositeContext *);
extern void gimp_composite_dissolve_rgba8_rgba8_rgba8_sse (GimpCompositeContext *);
extern void gimp_composite_divide_rgba8_rgba8_rgba8_sse (GimpCompositeContext *);
extern void gimp_composite_dodge_rgba8_rgba8_rgba8_sse (GimpCompositeContext *);
extern void gimp_composite_grain_extract_rgba8_rgba8_rgba8_sse (GimpCompositeContext *);
extern void gimp_composite_grain_merge_rgba8_rgba8_rgba8_sse (GimpCompositeContext *);
extern void gimp_composite_hardlight_rgba8_rgba8_rgba8_sse (GimpCompositeContext *);
extern void gimp_composite_hueonly_rgba8_rgba8_rgba8_sse (GimpCompositeContext *);
extern void gimp_composite_lighten_rgba8_rgba8_rgba8_sse (GimpCompositeContext *);
extern void gimp_composite_multiply_rgba8_rgba8_rgba8_sse (GimpCompositeContext *);
extern void gimp_composite_overlay_rgba8_rgba8_rgba8_sse (GimpCompositeContext *);
extern void gimp_composite_replace_rgba8_rgba8_rgba8_sse (GimpCompositeContext *);
extern void gimp_composite_saturationonly_rgba8_rgba8_rgba8_sse (GimpCompositeContext *);
extern void gimp_composite_screen_rgba8_rgba8_rgba8_sse (GimpCompositeContext *);
extern void gimp_composite_softlight_rgba8_rgba8_rgba8_sse (GimpCompositeContext *);
extern void gimp_composite_subtract_rgba8_rgba8_rgba8_sse (GimpCompositeContext *);
extern void gimp_composite_swap_rgba8_rgba8_rgba8_sse (GimpCompositeContext *);
extern void gimp_composite_valueonly_rgba8_rgba8_rgba8_sse (GimpCompositeContext *);
extern void gimp_composite_scale_rgba8_rgba8_rgba8_sse (GimpCompositeContext *);

#endif /* __GNUC__ > 3 */
#endif /* ARCH_X86 */
#endif /* USE_MMX */
#endif
