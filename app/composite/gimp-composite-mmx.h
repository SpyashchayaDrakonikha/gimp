#ifndef gimp_composite_mmx_h
#define gimp_composite_mmx_h

extern gboolean gimp_composite_mmx_init (void);

/*
	* The function gimp_composite_*_install() is defined in the code
	* generated by make-install.py I hate to have make-install.py create
	* a .h file just for that declaration, so I do it here.
 */
extern gboolean gimp_composite_mmx_install (void);

/*
 * Rummage through the compile-time environment to ensure this file
 * can actually be compiled like we expect it to.  If all is okay,
 * define the manifest constant COMPILE_MMX_IS_OKAY.
 */
#if !defined(__INTEL_COMPILER) || defined(USE_INTEL_COMPILER_ANYWAY)
#if defined(USE_MMX)
#if defined(ARCH_X86)
#if __GNUC__ >= 3
#if defined(ARCH_X86_64) || (!defined(PIC) && !defined(__PIC__))
#define COMPILE_MMX_IS_OKAY (1)
#endif /* defined(ARCH_X86_64) || (!defined(PIC) && !defined(__PIC__)) */
#endif /* __GNUC__ >= 3 */
#endif /* defined(ARCH_X86) */
#endif /* defined(USE_MMX) */
#endif /* !defined(__INTEL_COMPILER) */


#ifdef COMPILE_MMX_IS_OKAY
/*
 *
 */
extern void gimp_composite_addition_rgba8_rgba8_rgba8_mmx (GimpCompositeContext *ctx);
extern void gimp_composite_burn_rgba8_rgba8_rgba8_mmx (GimpCompositeContext *ctx);
extern void gimp_composite_coloronly_rgba8_rgba8_rgba8_mmx (GimpCompositeContext *ctx);
extern void gimp_composite_darken_rgba8_rgba8_rgba8_mmx (GimpCompositeContext *ctx);
extern void gimp_composite_difference_rgba8_rgba8_rgba8_mmx (GimpCompositeContext *ctx);
extern void gimp_composite_dissolve_rgba8_rgba8_rgba8_mmx (GimpCompositeContext *ctx);
extern void gimp_composite_divide_rgba8_rgba8_rgba8_mmx (GimpCompositeContext *ctx);
extern void gimp_composite_dodge_rgba8_rgba8_rgba8_mmx (GimpCompositeContext *ctx);
extern void gimp_composite_grain_extract_rgba8_rgba8_rgba8_mmx (GimpCompositeContext *ctx);
extern void gimp_composite_grain_merge_rgba8_rgba8_rgba8_mmx (GimpCompositeContext *ctx);
extern void gimp_composite_hardlight_rgba8_rgba8_rgba8_mmx (GimpCompositeContext *ctx);
extern void gimp_composite_hueonly_rgba8_rgba8_rgba8_mmx (GimpCompositeContext *ctx);
extern void gimp_composite_lighten_rgba8_rgba8_rgba8_mmx (GimpCompositeContext *ctx);
extern void gimp_composite_multiply_rgba8_rgba8_rgba8_mmx (GimpCompositeContext *ctx);
extern void gimp_composite_overlay_rgba8_rgba8_rgba8_mmx (GimpCompositeContext *ctx);
extern void gimp_composite_replace_rgba8_rgba8_rgba8_mmx (GimpCompositeContext *ctx);
extern void gimp_composite_saturationonly_rgba8_rgba8_rgba8_mmx (GimpCompositeContext *ctx);
extern void gimp_composite_scale_rgba8_rgba8_rgba8_mmx (GimpCompositeContext *ctx);
extern void gimp_composite_screen_rgba8_rgba8_rgba8_mmx (GimpCompositeContext *ctx);
extern void gimp_composite_softlight_rgba8_rgba8_rgba8_mmx (GimpCompositeContext *ctx);
extern void gimp_composite_subtract_rgba8_rgba8_rgba8_mmx (GimpCompositeContext *ctx);
extern void gimp_composite_swap_rgba8_rgba8_rgba8_mmx (GimpCompositeContext *ctx);
extern void gimp_composite_valueonly_rgba8_rgba8_rgba8_mmx (GimpCompositeContext *ctx);

extern void gimp_composite_addition_va8_va8_va8_mmx (GimpCompositeContext *ctx);
extern void gimp_composite_subtract_va8_va8_va8_mmx (GimpCompositeContext *ctx);
/*extern void gimp_composite_multiply_va8_va8_va8_mmx (GimpCompositeContext *ctx);*/
#endif /* COMPILE_IS_OKAY */
#endif
