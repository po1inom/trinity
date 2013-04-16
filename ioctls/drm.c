#include "config.h"

#ifdef USE_DRM

#include <stdlib.h>
#include <inttypes.h>

#include <drm/drm.h>
#ifdef USE_DRM_EXYNOS
#include <drm/exynos_drm.h>
#endif
#include <drm/i810_drm.h>
#include <drm/i915_drm.h>
#include <drm/mga_drm.h>
#include <drm/nouveau_drm.h>
#include <drm/r128_drm.h>
#include <drm/radeon_drm.h>
#include <drm/savage_drm.h>

#include "trinity.h"
#include "ioctls.h"

/* Copy-paste relevant stuff from header, as it is buggy. */
/* #include <drm/sis_drm.h> */

/* SiS specific ioctls */
#define NOT_USED_0_3
#define DRM_SIS_FB_ALLOC	0x04
#define DRM_SIS_FB_FREE	        0x05
#define NOT_USED_6_12
#define DRM_SIS_AGP_INIT	0x13
#define DRM_SIS_AGP_ALLOC	0x14
#define DRM_SIS_AGP_FREE	0x15
#define DRM_SIS_FB_INIT	        0x16

#define DRM_IOCTL_SIS_FB_ALLOC		DRM_IOWR(DRM_COMMAND_BASE + DRM_SIS_FB_ALLOC, drm_sis_mem_t)
#define DRM_IOCTL_SIS_FB_FREE		DRM_IOW( DRM_COMMAND_BASE + DRM_SIS_FB_FREE, drm_sis_mem_t)
#define DRM_IOCTL_SIS_AGP_INIT		DRM_IOWR(DRM_COMMAND_BASE + DRM_SIS_AGP_INIT, drm_sis_agp_t)
#define DRM_IOCTL_SIS_AGP_ALLOC		DRM_IOWR(DRM_COMMAND_BASE + DRM_SIS_AGP_ALLOC, drm_sis_mem_t)
#define DRM_IOCTL_SIS_AGP_FREE		DRM_IOW( DRM_COMMAND_BASE + DRM_SIS_AGP_FREE, drm_sis_mem_t)
#define DRM_IOCTL_SIS_FB_INIT		DRM_IOW( DRM_COMMAND_BASE + DRM_SIS_FB_INIT, drm_sis_fb_t)
/*
#define DRM_IOCTL_SIS_FLIP		DRM_IOW( 0x48, drm_sis_flip_t)
#define DRM_IOCTL_SIS_FLIP_INIT		DRM_IO(  0x49)
#define DRM_IOCTL_SIS_FLIP_FINAL	DRM_IO(  0x50)
*/

typedef struct {
	int context;
	unsigned long offset;
	unsigned long size;
	unsigned long free;
} drm_sis_mem_t;

typedef struct {
	unsigned long offset, size;
} drm_sis_agp_t;

typedef struct {
	unsigned long offset, size;
} drm_sis_fb_t;

/* header is buggy. */
/* #include <drm/via_drm.h> */

static const struct ioctl drm_ioctls[] = {
	/* drm/drm.h */
	IOCTL(DRM_IOCTL_VERSION),
	IOCTL(DRM_IOCTL_GET_UNIQUE),
	IOCTL(DRM_IOCTL_GET_MAGIC),
	IOCTL(DRM_IOCTL_IRQ_BUSID),
	IOCTL(DRM_IOCTL_GET_MAP),
	IOCTL(DRM_IOCTL_GET_CLIENT),
	IOCTL(DRM_IOCTL_GET_STATS),
	IOCTL(DRM_IOCTL_SET_VERSION),
	IOCTL(DRM_IOCTL_MODESET_CTL),
	IOCTL(DRM_IOCTL_GEM_CLOSE),
	IOCTL(DRM_IOCTL_GEM_FLINK),
	IOCTL(DRM_IOCTL_GEM_OPEN),
#ifdef DRM_IOCTL_GET_CAP
	IOCTL(DRM_IOCTL_GET_CAP),
#endif
	IOCTL(DRM_IOCTL_SET_UNIQUE),
	IOCTL(DRM_IOCTL_AUTH_MAGIC),
	IOCTL(DRM_IOCTL_BLOCK),
	IOCTL(DRM_IOCTL_UNBLOCK),
	IOCTL(DRM_IOCTL_CONTROL),
	IOCTL(DRM_IOCTL_ADD_MAP),
	IOCTL(DRM_IOCTL_ADD_BUFS),
	IOCTL(DRM_IOCTL_MARK_BUFS),
	IOCTL(DRM_IOCTL_INFO_BUFS),
	IOCTL(DRM_IOCTL_MAP_BUFS),
	IOCTL(DRM_IOCTL_FREE_BUFS),
	IOCTL(DRM_IOCTL_RM_MAP),
	IOCTL(DRM_IOCTL_SET_SAREA_CTX),
	IOCTL(DRM_IOCTL_GET_SAREA_CTX),
	IOCTL(DRM_IOCTL_SET_MASTER),
	IOCTL(DRM_IOCTL_DROP_MASTER),
	IOCTL(DRM_IOCTL_ADD_CTX),
	IOCTL(DRM_IOCTL_RM_CTX),
	IOCTL(DRM_IOCTL_MOD_CTX),
	IOCTL(DRM_IOCTL_GET_CTX),
	IOCTL(DRM_IOCTL_SWITCH_CTX),
	IOCTL(DRM_IOCTL_NEW_CTX),
	IOCTL(DRM_IOCTL_RES_CTX),
	IOCTL(DRM_IOCTL_ADD_DRAW),
	IOCTL(DRM_IOCTL_RM_DRAW),
	IOCTL(DRM_IOCTL_DMA),
	IOCTL(DRM_IOCTL_LOCK),
	IOCTL(DRM_IOCTL_UNLOCK),
	IOCTL(DRM_IOCTL_FINISH),
#ifdef DRM_IOCTL_PRIME_HANDLE_TO_FD
	IOCTL(DRM_IOCTL_PRIME_HANDLE_TO_FD),
#endif
#ifdef DRM_IOCTL_PRIME_FD_TO_HANDLE
	IOCTL(DRM_IOCTL_PRIME_FD_TO_HANDLE),
#endif
	IOCTL(DRM_IOCTL_AGP_ACQUIRE),
	IOCTL(DRM_IOCTL_AGP_RELEASE),
	IOCTL(DRM_IOCTL_AGP_ENABLE),
	IOCTL(DRM_IOCTL_AGP_INFO),
	IOCTL(DRM_IOCTL_AGP_ALLOC),
	IOCTL(DRM_IOCTL_AGP_FREE),
	IOCTL(DRM_IOCTL_AGP_BIND),
	IOCTL(DRM_IOCTL_AGP_UNBIND),
	IOCTL(DRM_IOCTL_SG_ALLOC),
	IOCTL(DRM_IOCTL_SG_FREE),
	IOCTL(DRM_IOCTL_WAIT_VBLANK),
	IOCTL(DRM_IOCTL_UPDATE_DRAW),
	IOCTL(DRM_IOCTL_MODE_GETRESOURCES),
	IOCTL(DRM_IOCTL_MODE_GETCRTC),
	IOCTL(DRM_IOCTL_MODE_SETCRTC),
	IOCTL(DRM_IOCTL_MODE_CURSOR),
	IOCTL(DRM_IOCTL_MODE_GETGAMMA),
	IOCTL(DRM_IOCTL_MODE_SETGAMMA),
	IOCTL(DRM_IOCTL_MODE_GETENCODER),
	IOCTL(DRM_IOCTL_MODE_GETCONNECTOR),
	IOCTL(DRM_IOCTL_MODE_ATTACHMODE),
	IOCTL(DRM_IOCTL_MODE_DETACHMODE),
	IOCTL(DRM_IOCTL_MODE_GETPROPERTY),
	IOCTL(DRM_IOCTL_MODE_SETPROPERTY),
	IOCTL(DRM_IOCTL_MODE_GETPROPBLOB),
	IOCTL(DRM_IOCTL_MODE_GETFB),
	IOCTL(DRM_IOCTL_MODE_ADDFB),
	IOCTL(DRM_IOCTL_MODE_RMFB),
	IOCTL(DRM_IOCTL_MODE_PAGE_FLIP),
	IOCTL(DRM_IOCTL_MODE_DIRTYFB),
#ifdef DRM_IOCTL_MODE_CREATE_DUMB
	IOCTL(DRM_IOCTL_MODE_CREATE_DUMB),
#endif
#ifdef DRM_IOCTL_MODE_MAP_DUMB
	IOCTL(DRM_IOCTL_MODE_MAP_DUMB),
#endif
#ifdef DRM_IOCTL_MODE_DESTROY_DUMB
	IOCTL(DRM_IOCTL_MODE_DESTROY_DUMB),
#endif
#ifdef DRM_IOCTL_MODE_GETPLANERESOURCES
	IOCTL(DRM_IOCTL_MODE_GETPLANERESOURCES),
#endif
#ifdef DRM_IOCTL_MODE_GETPLANE
	IOCTL(DRM_IOCTL_MODE_GETPLANE),
#endif
#ifdef DRM_IOCTL_MODE_SETPLANE
	IOCTL(DRM_IOCTL_MODE_SETPLANE),
#endif
#ifdef DRM_IOCTL_MODE_ADDFB2
	IOCTL(DRM_IOCTL_MODE_ADDFB2),
#endif
#ifdef DRM_IOCTL_MODE_OBJ_GETPROPERTIES
	IOCTL(DRM_IOCTL_MODE_OBJ_GETPROPERTIES),
#endif
#ifdef DRM_IOCTL_MODE_OBJ_SETPROPERTY
	IOCTL(DRM_IOCTL_MODE_OBJ_SETPROPERTY),
#endif

#ifdef USE_DRM_EXYNOS
	/* exynos_drm.h */
	IOCTL(DRM_IOCTL_EXYNOS_GEM_CREATE),
	IOCTL(DRM_IOCTL_EXYNOS_GEM_MAP_OFFSET),
	IOCTL(DRM_IOCTL_EXYNOS_GEM_MMAP),
#ifdef DRM_IOCTL_EXYNOS_GEM_GET
	IOCTL(DRM_IOCTL_EXYNOS_GEM_GET),
#endif
	IOCTL(DRM_IOCTL_EXYNOS_VIDI_CONNECTION),
#ifdef DRM_IOCTL_EXYNOS_G2D_GET_VER
	IOCTL(DRM_IOCTL_EXYNOS_G2D_GET_VER),
#endif
#ifdef DRM_IOCTL_EXYNOS_G2D_SET_CMDLIST
	IOCTL(DRM_IOCTL_EXYNOS_G2D_SET_CMDLIST),
#endif
#ifdef DRM_IOCTL_EXYNOS_G2D_EXEC
	IOCTL(DRM_IOCTL_EXYNOS_G2D_EXEC),
#endif
#endif

	/* i810_drm.h */
	IOCTL(DRM_IOCTL_I810_INIT),
	IOCTL(DRM_IOCTL_I810_VERTEX),
	IOCTL(DRM_IOCTL_I810_CLEAR),
	IOCTL(DRM_IOCTL_I810_FLUSH),
	IOCTL(DRM_IOCTL_I810_GETAGE),
	IOCTL(DRM_IOCTL_I810_GETBUF),
	IOCTL(DRM_IOCTL_I810_SWAP),
	IOCTL(DRM_IOCTL_I810_COPY),
	IOCTL(DRM_IOCTL_I810_DOCOPY),
	IOCTL(DRM_IOCTL_I810_OV0INFO),
	IOCTL(DRM_IOCTL_I810_FSTATUS),
	IOCTL(DRM_IOCTL_I810_OV0FLIP),
	IOCTL(DRM_IOCTL_I810_MC),
	IOCTL(DRM_IOCTL_I810_RSTATUS),
	IOCTL(DRM_IOCTL_I810_FLIP),

	/* i915_drm.h */
	IOCTL(DRM_IOCTL_I915_INIT),
	IOCTL(DRM_IOCTL_I915_FLUSH),
	IOCTL(DRM_IOCTL_I915_FLIP),
	IOCTL(DRM_IOCTL_I915_BATCHBUFFER),
	IOCTL(DRM_IOCTL_I915_IRQ_EMIT),
	IOCTL(DRM_IOCTL_I915_IRQ_WAIT),
	IOCTL(DRM_IOCTL_I915_GETPARAM),
	IOCTL(DRM_IOCTL_I915_SETPARAM),
	IOCTL(DRM_IOCTL_I915_ALLOC),
	IOCTL(DRM_IOCTL_I915_FREE),
	IOCTL(DRM_IOCTL_I915_INIT_HEAP),
	IOCTL(DRM_IOCTL_I915_CMDBUFFER),
	IOCTL(DRM_IOCTL_I915_DESTROY_HEAP),
	IOCTL(DRM_IOCTL_I915_SET_VBLANK_PIPE),
	IOCTL(DRM_IOCTL_I915_GET_VBLANK_PIPE),
	IOCTL(DRM_IOCTL_I915_VBLANK_SWAP),
#ifdef DRM_IOCTL_I915_HWS_ADDR
	IOCTL(DRM_IOCTL_I915_HWS_ADDR),
#endif
	IOCTL(DRM_IOCTL_I915_GEM_INIT),
	IOCTL(DRM_IOCTL_I915_GEM_EXECBUFFER),
	IOCTL(DRM_IOCTL_I915_GEM_EXECBUFFER2),
	IOCTL(DRM_IOCTL_I915_GEM_PIN),
	IOCTL(DRM_IOCTL_I915_GEM_UNPIN),
	IOCTL(DRM_IOCTL_I915_GEM_BUSY),
#ifdef DRM_IOCTL_I915_GEM_SET_CACHING
	IOCTL(DRM_IOCTL_I915_GEM_SET_CACHING),
#endif
#ifdef DRM_IOCTL_I915_GEM_GET_CACHING
	IOCTL(DRM_IOCTL_I915_GEM_GET_CACHING),
#endif
	IOCTL(DRM_IOCTL_I915_GEM_THROTTLE),
	IOCTL(DRM_IOCTL_I915_GEM_ENTERVT),
	IOCTL(DRM_IOCTL_I915_GEM_LEAVEVT),
	IOCTL(DRM_IOCTL_I915_GEM_CREATE),
	IOCTL(DRM_IOCTL_I915_GEM_PREAD),
	IOCTL(DRM_IOCTL_I915_GEM_PWRITE),
	IOCTL(DRM_IOCTL_I915_GEM_MMAP),
	IOCTL(DRM_IOCTL_I915_GEM_MMAP_GTT),
	IOCTL(DRM_IOCTL_I915_GEM_SET_DOMAIN),
	IOCTL(DRM_IOCTL_I915_GEM_SW_FINISH),
	IOCTL(DRM_IOCTL_I915_GEM_SET_TILING),
	IOCTL(DRM_IOCTL_I915_GEM_GET_TILING),
	IOCTL(DRM_IOCTL_I915_GEM_GET_APERTURE),
	IOCTL(DRM_IOCTL_I915_GET_PIPE_FROM_CRTC_ID),
	IOCTL(DRM_IOCTL_I915_GEM_MADVISE),
	IOCTL(DRM_IOCTL_I915_OVERLAY_PUT_IMAGE),
	IOCTL(DRM_IOCTL_I915_OVERLAY_ATTRS),
#ifdef DRM_IOCTL_I915_SET_SPRITE_COLORKEY
	IOCTL(DRM_IOCTL_I915_SET_SPRITE_COLORKEY),
#endif
#ifdef DRM_IOCTL_I915_GET_SPRITE_COLORKEY
	IOCTL(DRM_IOCTL_I915_GET_SPRITE_COLORKEY),
#endif
#ifdef DRM_IOCTL_I915_GEM_WAIT
	IOCTL(DRM_IOCTL_I915_GEM_WAIT),
#endif
#ifdef DRM_IOCTL_I915_GEM_CONTEXT_CREATE
	IOCTL(DRM_IOCTL_I915_GEM_CONTEXT_CREATE),
#endif
#ifdef DRM_IOCTL_I915_GEM_CONTEXT_DESTROY
	IOCTL(DRM_IOCTL_I915_GEM_CONTEXT_DESTROY),
#endif
#ifdef DRM_IOCTL_I915_REG_READ
	IOCTL(DRM_IOCTL_I915_REG_READ),
#endif

	/* mga_drm.h */
	IOCTL(DRM_IOCTL_MGA_INIT),
	IOCTL(DRM_IOCTL_MGA_FLUSH),
	IOCTL(DRM_IOCTL_MGA_RESET),
	IOCTL(DRM_IOCTL_MGA_SWAP),
	IOCTL(DRM_IOCTL_MGA_CLEAR),
	IOCTL(DRM_IOCTL_MGA_VERTEX),
	IOCTL(DRM_IOCTL_MGA_INDICES),
	IOCTL(DRM_IOCTL_MGA_ILOAD),
	IOCTL(DRM_IOCTL_MGA_BLIT),
	IOCTL(DRM_IOCTL_MGA_GETPARAM),
	IOCTL(DRM_IOCTL_MGA_SET_FENCE),
	IOCTL(DRM_IOCTL_MGA_WAIT_FENCE),
	IOCTL(DRM_IOCTL_MGA_DMA_BOOTSTRAP),

	/* nouveau_drm.h */
#ifdef DRM_IOCTL_NOUVEAU_GEM_NEW
	IOCTL(DRM_IOCTL_NOUVEAU_GEM_NEW),
#endif
#ifdef DRM_IOCTL_NOUVEAU_GEM_PUSHBUF
	IOCTL(DRM_IOCTL_NOUVEAU_GEM_PUSHBUF),
#endif
#ifdef DRM_IOCTL_NOUVEAU_GEM_CPU_PREP
	IOCTL(DRM_IOCTL_NOUVEAU_GEM_CPU_PREP),
#endif
#ifdef DRM_IOCTL_NOUVEAU_GEM_CPU_FINI
	IOCTL(DRM_IOCTL_NOUVEAU_GEM_CPU_FINI),
#endif
#ifdef DRM_IOCTL_NOUVEAU_GEM_INFO
	IOCTL(DRM_IOCTL_NOUVEAU_GEM_INFO),
#endif

	/* r128_drm.h */
	IOCTL(DRM_IOCTL_R128_INIT),
	IOCTL(DRM_IOCTL_R128_CCE_START),
	IOCTL(DRM_IOCTL_R128_CCE_STOP),
	IOCTL(DRM_IOCTL_R128_CCE_RESET),
	IOCTL(DRM_IOCTL_R128_CCE_IDLE),
	IOCTL(DRM_IOCTL_R128_RESET),
	IOCTL(DRM_IOCTL_R128_SWAP),
	IOCTL(DRM_IOCTL_R128_CLEAR),
	IOCTL(DRM_IOCTL_R128_VERTEX),
	IOCTL(DRM_IOCTL_R128_INDICES),
	IOCTL(DRM_IOCTL_R128_BLIT),
	IOCTL(DRM_IOCTL_R128_DEPTH),
	IOCTL(DRM_IOCTL_R128_STIPPLE),
	IOCTL(DRM_IOCTL_R128_INDIRECT),
	IOCTL(DRM_IOCTL_R128_FULLSCREEN),
	IOCTL(DRM_IOCTL_R128_GETPARAM),
	IOCTL(DRM_IOCTL_R128_FLIP),

	/* radeon_drm.h */
	IOCTL(DRM_IOCTL_RADEON_CP_INIT),
	IOCTL(DRM_IOCTL_RADEON_CP_START),
	IOCTL(DRM_IOCTL_RADEON_CP_STOP),
	IOCTL(DRM_IOCTL_RADEON_CP_RESET),
	IOCTL(DRM_IOCTL_RADEON_CP_IDLE),
	IOCTL(DRM_IOCTL_RADEON_RESET),
	IOCTL(DRM_IOCTL_RADEON_FULLSCREEN),
	IOCTL(DRM_IOCTL_RADEON_SWAP),
	IOCTL(DRM_IOCTL_RADEON_CLEAR),
	IOCTL(DRM_IOCTL_RADEON_VERTEX),
	IOCTL(DRM_IOCTL_RADEON_INDICES),
	IOCTL(DRM_IOCTL_RADEON_STIPPLE),
	IOCTL(DRM_IOCTL_RADEON_INDIRECT),
	IOCTL(DRM_IOCTL_RADEON_TEXTURE),
	IOCTL(DRM_IOCTL_RADEON_VERTEX2),
	IOCTL(DRM_IOCTL_RADEON_CMDBUF),
	IOCTL(DRM_IOCTL_RADEON_GETPARAM),
	IOCTL(DRM_IOCTL_RADEON_FLIP),
	IOCTL(DRM_IOCTL_RADEON_ALLOC),
	IOCTL(DRM_IOCTL_RADEON_FREE),
	IOCTL(DRM_IOCTL_RADEON_INIT_HEAP),
	IOCTL(DRM_IOCTL_RADEON_IRQ_EMIT),
	IOCTL(DRM_IOCTL_RADEON_IRQ_WAIT),
	IOCTL(DRM_IOCTL_RADEON_CP_RESUME),
	IOCTL(DRM_IOCTL_RADEON_SETPARAM),
	IOCTL(DRM_IOCTL_RADEON_SURF_ALLOC),
	IOCTL(DRM_IOCTL_RADEON_SURF_FREE),
	IOCTL(DRM_IOCTL_RADEON_GEM_INFO),
	IOCTL(DRM_IOCTL_RADEON_GEM_CREATE),
	IOCTL(DRM_IOCTL_RADEON_GEM_MMAP),
	IOCTL(DRM_IOCTL_RADEON_GEM_PREAD),
	IOCTL(DRM_IOCTL_RADEON_GEM_PWRITE),
	IOCTL(DRM_IOCTL_RADEON_GEM_SET_DOMAIN),
	IOCTL(DRM_IOCTL_RADEON_GEM_WAIT_IDLE),
	IOCTL(DRM_IOCTL_RADEON_CS),
	IOCTL(DRM_IOCTL_RADEON_INFO),
#ifdef DRM_IOCTL_RADEON_GEM_SET_TILING
	IOCTL(DRM_IOCTL_RADEON_GEM_SET_TILING),
#endif
#ifdef DRM_IOCTL_RADEON_GEM_GET_TILING
	IOCTL(DRM_IOCTL_RADEON_GEM_GET_TILING),
#endif
	IOCTL(DRM_IOCTL_RADEON_GEM_BUSY),
#ifdef DRM_IOCTL_RADEON_GEM_VA
	IOCTL(DRM_IOCTL_RADEON_GEM_VA),
#endif

	/* savage_drm.h */
#ifdef DRM_IOCTL_SAVAGE_BCI_INIT
	IOCTL(DRM_IOCTL_SAVAGE_BCI_INIT),
#endif
#ifdef DRM_IOCTL_SAVAGE_BCI_CMDBUF
	IOCTL(DRM_IOCTL_SAVAGE_BCI_CMDBUF),
#endif
#ifdef DRM_IOCTL_SAVAGE_BCI_EVENT_EMIT
	IOCTL(DRM_IOCTL_SAVAGE_BCI_EVENT_EMIT),
#endif
#ifdef DRM_IOCTL_SAVAGE_BCI_EVENT_WAIT
	IOCTL(DRM_IOCTL_SAVAGE_BCI_EVENT_WAIT),
#endif

	/* sis_drm.h */
	IOCTL(DRM_IOCTL_SIS_FB_ALLOC),
	IOCTL(DRM_IOCTL_SIS_FB_FREE),
	IOCTL(DRM_IOCTL_SIS_AGP_INIT),
	IOCTL(DRM_IOCTL_SIS_AGP_ALLOC),
	IOCTL(DRM_IOCTL_SIS_AGP_FREE),
	IOCTL(DRM_IOCTL_SIS_FB_INIT),

	/* via_drm.h */
	/*
	IOCTL(DRM_IOCTL_VIA_ALLOCMEM),
	IOCTL(DRM_IOCTL_VIA_FREEMEM),
	IOCTL(DRM_IOCTL_VIA_AGP_INIT),
	IOCTL(DRM_IOCTL_VIA_FB_INIT),
	IOCTL(DRM_IOCTL_VIA_MAP_INIT),
	IOCTL(DRM_IOCTL_VIA_DEC_FUTEX),
	IOCTL(DRM_IOCTL_VIA_DMA_INIT),
	IOCTL(DRM_IOCTL_VIA_CMDBUFFER),
	IOCTL(DRM_IOCTL_VIA_FLUSH),
	IOCTL(DRM_IOCTL_VIA_PCICMD),
	IOCTL(DRM_IOCTL_VIA_CMDBUF_SIZE),
	IOCTL(DRM_IOCTL_VIA_WAIT_IRQ),
	IOCTL(DRM_IOCTL_VIA_DMA_BLIT),
	IOCTL(DRM_IOCTL_VIA_BLIT_SYNC),
	*/
};

static const char *const drm_devs[] = {
	"drm",
};

static const struct ioctl_group drm_grp = {
	.devtype = DEV_CHAR,
	.devs = drm_devs,
	.devs_cnt = ARRAY_SIZE(drm_devs),
	.sanitise = pick_random_ioctl,
	.ioctls = drm_ioctls,
	.ioctls_cnt = ARRAY_SIZE(drm_ioctls),
};

REG_IOCTL_GROUP(drm_grp)

#endif /* USE_DRM */