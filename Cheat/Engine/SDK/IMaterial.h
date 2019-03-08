#pragma once

#include "SDK.h"
#include "IMaterialSystem.h"

namespace SDK
{
	class ImageFormat;
	class IMaterialVar;

	typedef uint64 VertexFormat_t;

	enum VertexFormatFlags_t
	{
		// Indicates an uninitialized VertexFormat_t value
		VERTEX_FORMAT_INVALID = 0xFFFFFFFF,

		VERTEX_POSITION = 0x0001,
		VERTEX_NORMAL = 0x0002,
		VERTEX_COLOR = 0x0004,
		VERTEX_SPECULAR = 0x0008,

		VERTEX_TANGENT_S = 0x0010,
		VERTEX_TANGENT_T = 0x0020,
		VERTEX_TANGENT_SPACE = VERTEX_TANGENT_S | VERTEX_TANGENT_T,

		// Indicates we're using wrinkle
		VERTEX_WRINKLE = 0x0040,

		// Indicates we're using bone indices
		VERTEX_BONE_INDEX = 0x0080,

		// Indicates this expects a color stream on stream 1
		VERTEX_COLOR_STREAM_1 = 0x0100,

		// Indicates this format shouldn't be bloated to cache align it
		// (only used for VertexUsage)
		VERTEX_FORMAT_USE_EXACT_FORMAT = 0x0200,

		// Indicates that compressed vertex elements are to be used (see also VertexCompressionType_t)
		VERTEX_FORMAT_COMPRESSED = 0x400,

		// Position or normal (if present) should be 4D not 3D
		VERTEX_FORMAT_PAD_POS_NORM = 0x800,

		// Update this if you add or remove bits...
		VERTEX_LAST_BIT = 11,

		VERTEX_BONE_WEIGHT_BIT = VERTEX_LAST_BIT + 1,
		USER_DATA_SIZE_BIT = VERTEX_LAST_BIT + 4,
		TEX_COORD_SIZE_BIT = VERTEX_LAST_BIT + 7,

		VERTEX_BONE_WEIGHT_MASK = (0x7 << VERTEX_BONE_WEIGHT_BIT),
		USER_DATA_SIZE_MASK = (0x7 << USER_DATA_SIZE_BIT),

		VERTEX_FORMAT_FIELD_MASK = 0x0FF,

		// If everything is off, it's an unknown vertex format
		VERTEX_FORMAT_UNKNOWN = 0,
	};

#define VERTEX_BONEWEIGHT( _n )				((_n) << VERTEX_BONE_WEIGHT_BIT)
#define VERTEX_USERDATA_SIZE( _n )			((_n) << USER_DATA_SIZE_BIT)
#define VERTEX_TEXCOORD_MASK( _coord )		(( 0x7ULL ) << ( TEX_COORD_SIZE_BIT + 3 * (_coord) ))

	inline VertexFormat_t VERTEX_TEXCOORD_SIZE(int nIndex, int nNumCoords)
	{
		uint64 n64 = nNumCoords;
		uint64 nShift = TEX_COORD_SIZE_BIT + (3 * nIndex);
		return n64 << nShift;
	}

	enum VertexElement_t
	{
		VERTEX_ELEMENT_NONE = -1,

		// Deliberately explicitly numbered so it's a pain in the ass to change, so you read this:
		// #!#!#NOTE#!#!# update GetVertexElementSize, VertexElementToDeclType and
		//                CVBAllocTracker (elementTable) when you update this!
		VERTEX_ELEMENT_POSITION = 0,
		VERTEX_ELEMENT_POSITION4D = 1,
		VERTEX_ELEMENT_NORMAL = 2,
		VERTEX_ELEMENT_NORMAL4D = 3,
		VERTEX_ELEMENT_COLOR = 4,
		VERTEX_ELEMENT_SPECULAR = 5,
		VERTEX_ELEMENT_TANGENT_S = 6,
		VERTEX_ELEMENT_TANGENT_T = 7,
		VERTEX_ELEMENT_WRINKLE = 8,
		VERTEX_ELEMENT_BONEINDEX = 9,
		VERTEX_ELEMENT_BONEWEIGHTS1 = 10,
		VERTEX_ELEMENT_BONEWEIGHTS2 = 11,
		VERTEX_ELEMENT_BONEWEIGHTS3 = 12,
		VERTEX_ELEMENT_BONEWEIGHTS4 = 13,
		VERTEX_ELEMENT_USERDATA1 = 14,
		VERTEX_ELEMENT_USERDATA2 = 15,
		VERTEX_ELEMENT_USERDATA3 = 16,
		VERTEX_ELEMENT_USERDATA4 = 17,
		VERTEX_ELEMENT_TEXCOORD1D_0 = 18,
		VERTEX_ELEMENT_TEXCOORD1D_1 = 19,
		VERTEX_ELEMENT_TEXCOORD1D_2 = 20,
		VERTEX_ELEMENT_TEXCOORD1D_3 = 21,
		VERTEX_ELEMENT_TEXCOORD1D_4 = 22,
		VERTEX_ELEMENT_TEXCOORD1D_5 = 23,
		VERTEX_ELEMENT_TEXCOORD1D_6 = 24,
		VERTEX_ELEMENT_TEXCOORD1D_7 = 25,
		VERTEX_ELEMENT_TEXCOORD2D_0 = 26,
		VERTEX_ELEMENT_TEXCOORD2D_1 = 27,
		VERTEX_ELEMENT_TEXCOORD2D_2 = 28,
		VERTEX_ELEMENT_TEXCOORD2D_3 = 29,
		VERTEX_ELEMENT_TEXCOORD2D_4 = 30,
		VERTEX_ELEMENT_TEXCOORD2D_5 = 31,
		VERTEX_ELEMENT_TEXCOORD2D_6 = 32,
		VERTEX_ELEMENT_TEXCOORD2D_7 = 33,
		VERTEX_ELEMENT_TEXCOORD3D_0 = 34,
		VERTEX_ELEMENT_TEXCOORD3D_1 = 35,
		VERTEX_ELEMENT_TEXCOORD3D_2 = 36,
		VERTEX_ELEMENT_TEXCOORD3D_3 = 37,
		VERTEX_ELEMENT_TEXCOORD3D_4 = 38,
		VERTEX_ELEMENT_TEXCOORD3D_5 = 39,
		VERTEX_ELEMENT_TEXCOORD3D_6 = 40,
		VERTEX_ELEMENT_TEXCOORD3D_7 = 41,
		VERTEX_ELEMENT_TEXCOORD4D_0 = 42,
		VERTEX_ELEMENT_TEXCOORD4D_1 = 43,
		VERTEX_ELEMENT_TEXCOORD4D_2 = 44,
		VERTEX_ELEMENT_TEXCOORD4D_3 = 45,
		VERTEX_ELEMENT_TEXCOORD4D_4 = 46,
		VERTEX_ELEMENT_TEXCOORD4D_5 = 47,
		VERTEX_ELEMENT_TEXCOORD4D_6 = 48,
		VERTEX_ELEMENT_TEXCOORD4D_7 = 49,

		VERTEX_ELEMENT_NUMELEMENTS = 50
	};

#define COMPRESSED_NORMALS_SEPARATETANGENTS_SHORT2	0
#define COMPRESSED_NORMALS_COMBINEDTANGENTS_UBYTE4	1
#define COMPRESSED_NORMALS_TYPE	COMPRESSED_NORMALS_COMBINEDTANGENTS_UBYTE4

	enum MaterialVarFlags_t
	{
		MATERIAL_VAR_DEBUG = (1 << 0),
		MATERIAL_VAR_NO_DEBUG_OVERRIDE = (1 << 1),
		MATERIAL_VAR_NO_DRAW = (1 << 2),
		MATERIAL_VAR_USE_IN_FILLRATE_MODE = (1 << 3),
		MATERIAL_VAR_VERTEXCOLOR = (1 << 4),
		MATERIAL_VAR_VERTEXALPHA = (1 << 5),
		MATERIAL_VAR_SELFILLUM = (1 << 6),
		MATERIAL_VAR_ADDITIVE = (1 << 7),
		MATERIAL_VAR_ALPHATEST = (1 << 8),
		MATERIAL_VAR_ZNEARER = (1 << 10),
		MATERIAL_VAR_MODEL = (1 << 11),
		MATERIAL_VAR_FLAT = (1 << 12),
		MATERIAL_VAR_NOCULL = (1 << 13),
		MATERIAL_VAR_NOFOG = (1 << 14),
		MATERIAL_VAR_IGNOREZ = (1 << 15),
		MATERIAL_VAR_DECAL = (1 << 16),
		MATERIAL_VAR_ENVMAPSPHERE = (1 << 17), // OBSOLETE
		MATERIAL_VAR_ENVMAPCAMERASPACE = (1 << 19), // OBSOLETE
		MATERIAL_VAR_BASEALPHAENVMAPMASK = (1 << 20),
		MATERIAL_VAR_TRANSLUCENT = (1 << 21),
		MATERIAL_VAR_NORMALMAPALPHAENVMAPMASK = (1 << 22),
		MATERIAL_VAR_NEEDS_SOFTWARE_SKINNING = (1 << 23), // OBSOLETE
		MATERIAL_VAR_OPAQUETEXTURE = (1 << 24),
		MATERIAL_VAR_ENVMAPMODE = (1 << 25), // OBSOLETE
		MATERIAL_VAR_SUPPRESS_DECALS = (1 << 26),
		MATERIAL_VAR_HALFLAMBERT = (1 << 27),
		MATERIAL_VAR_WIREFRAME = (1 << 28),
		MATERIAL_VAR_ALLOWALPHATOCOVERAGE = (1 << 29),
		MATERIAL_VAR_ALPHA_MODIFIED_BY_PROXY = (1 << 30),
		MATERIAL_VAR_VERTEXFOG = (1 << 31),
	};

	enum MaterialVarFlags2_t
	{
		// NOTE: These are for $flags2!!!!!
		//	UNUSED											= (1 << 0),

		MATERIAL_VAR2_LIGHTING_UNLIT = 0,
		MATERIAL_VAR2_LIGHTING_VERTEX_LIT = (1 << 1),
		MATERIAL_VAR2_LIGHTING_LIGHTMAP = (1 << 2),
		MATERIAL_VAR2_LIGHTING_BUMPED_LIGHTMAP = (1 << 3),
		MATERIAL_VAR2_LIGHTING_MASK =
		(MATERIAL_VAR2_LIGHTING_VERTEX_LIT |
			MATERIAL_VAR2_LIGHTING_LIGHTMAP |
			MATERIAL_VAR2_LIGHTING_BUMPED_LIGHTMAP),

		// FIXME: Should this be a part of the above lighting enums?
		MATERIAL_VAR2_DIFFUSE_BUMPMAPPED_MODEL = (1 << 4),
		MATERIAL_VAR2_USES_ENV_CUBEMAP = (1 << 5),
		MATERIAL_VAR2_NEEDS_TANGENT_SPACES = (1 << 6),
		MATERIAL_VAR2_NEEDS_SOFTWARE_LIGHTING = (1 << 7),
		// GR - HDR path puts lightmap alpha in separate texture...
		MATERIAL_VAR2_BLEND_WITH_LIGHTMAP_ALPHA = (1 << 8),
		MATERIAL_VAR2_NEEDS_BAKED_LIGHTING_SNAPSHOTS = (1 << 9),
		MATERIAL_VAR2_USE_FLASHLIGHT = (1 << 10),
		MATERIAL_VAR2_USE_FIXED_FUNCTION_BAKED_LIGHTING = (1 << 11),
		MATERIAL_VAR2_NEEDS_FIXED_FUNCTION_FLASHLIGHT = (1 << 12),
		MATERIAL_VAR2_USE_EDITOR = (1 << 13),
		MATERIAL_VAR2_NEEDS_POWER_OF_TWO_FRAME_BUFFER_TEXTURE = (1 << 14),
		MATERIAL_VAR2_NEEDS_FULL_FRAME_BUFFER_TEXTURE = (1 << 15),
		MATERIAL_VAR2_IS_SPRITECARD = (1 << 16),
		MATERIAL_VAR2_USES_VERTEXID = (1 << 17),
		MATERIAL_VAR2_SUPPORTS_HW_SKINNING = (1 << 18),
		MATERIAL_VAR2_SUPPORTS_FLASHLIGHT = (1 << 19),
		MATERIAL_VAR2_USE_GBUFFER0 = (1 << 20),
		MATERIAL_VAR2_USE_GBUFFER1 = (1 << 21),
		MATERIAL_VAR2_SELFILLUMMASK = (1 << 22),
		MATERIAL_VAR2_SUPPORTS_TESSELLATION = (1 << 23)
	};

	enum PreviewImageRetVal_t
	{
		MATERIAL_PREVIEW_IMAGE_BAD = 0,
		MATERIAL_PREVIEW_IMAGE_OK,
		MATERIAL_NO_PREVIEW_IMAGE,
	};

class IMaterial
	{
	public:
		virtual const char*             GetName() const = 0;
		virtual const char*             GetTextureGroupName() const = 0;
		virtual PreviewImageRetVal_t    GetPreviewImageProperties(int *width, int *height, ImageFormat *imageFormat, bool* isTranslucent) const = 0;
		virtual PreviewImageRetVal_t    GetPreviewImage(unsigned char *data, int width, int height, ImageFormat imageFormat) const = 0;
		virtual int                     GetMappingWidth() = 0;
		virtual int                     GetMappingHeight() = 0;
		virtual int                     GetNumAnimationFrames() = 0;
		virtual bool                    InMaterialPage(void) = 0;
		virtual    void                 GetMaterialOffset(float *pOffset) = 0;
		virtual void                    GetMaterialScale(float *pScale) = 0;
		virtual IMaterial*              GetMaterialPage(void) = 0;
		virtual IMaterialVar*           FindVar(const char *varName, bool *found, bool complain = true) = 0;
		virtual void                    IncrementReferenceCount(void) = 0;
		virtual void                    DecrementReferenceCount(void) = 0;
		inline void                     AddRef() { IncrementReferenceCount(); }
		inline void                     Release() { DecrementReferenceCount(); }
		virtual int                     GetEnumerationID(void) const = 0;
		virtual void                    GetLowResColorSample(float s, float t, float *color) const = 0;
		virtual void                    RecomputeStateSnapshots() = 0;
		virtual bool                    IsTranslucent() = 0;
		virtual bool                    IsAlphaTested() = 0;
		virtual bool                    IsVertexLit() = 0;
		virtual VertexFormat_t          GetVertexFormat() const = 0;
		virtual bool                    HasProxy(void) const = 0;
		virtual bool                    UsesEnvCubemap(void) = 0;
		virtual bool                    NeedsTangentSpace(void) = 0;
		virtual bool                    NeedsPowerOfTwoFrameBufferTexture(bool bCheckSpecificToThisFrame = true) = 0;
		virtual bool                    NeedsFullFrameBufferTexture(bool bCheckSpecificToThisFrame = true) = 0;
		virtual bool                    NeedsSoftwareSkinning(void) = 0;
		virtual void                    AlphaModulate(float alpha) = 0;
		virtual void                    ColorModulate(float r, float g, float b) = 0;
		virtual void                    SetMaterialVarFlag(MaterialVarFlags_t flag, bool on) = 0;
		virtual bool                    GetMaterialVarFlag(MaterialVarFlags_t flag) const = 0;
		virtual void                    GetReflectivity(Vector& reflect) = 0;
		virtual bool                    GetPropertyFlag(MaterialPropertyTypes_t type) = 0;
		virtual bool                    IsTwoSided() = 0;
		virtual void                    SetShader(const char *pShaderName) = 0;
		virtual int                     GetNumPasses(void) = 0;
		virtual int                     GetTextureMemoryBytes(void) = 0;
		virtual void                    Refresh() = 0;
		virtual bool                    NeedsLightmapBlendAlpha(void) = 0;
		virtual bool                    NeedsSoftwareLighting(void) = 0;
		virtual int                     ShaderParamCount() const = 0;
		virtual IMaterialVar**          GetShaderParams(void) = 0;
		virtual bool                    IsErrorMaterial() const = 0;
		virtual void                    Unused() = 0;
		virtual float                   GetAlphaModulation() = 0;
		virtual void                    GetColorModulation(float *r, float *g, float *b) = 0;
		virtual bool                    IsTranslucentUnderModulation(float fAlphaModulation = 1.0f) const = 0;
		virtual IMaterialVar*           FindVarFast(char const *pVarName, unsigned int *pToken) = 0;
		virtual void                    SetShaderAndParams(KeyValues *pKeyValues) = 0;
		virtual const char*             GetShaderName() const = 0;
		virtual void                    DeleteIfUnreferenced() = 0;
		virtual bool                    IsSpriteCard() = 0;
		virtual void                    CallBindProxy(void *proxyData) = 0;
		virtual void                    RefreshPreservingMaterialVars() = 0;
		virtual bool                    WasReloadedFromWhitelist() = 0;
		virtual bool                    SetTempExcluded(bool bSet, int nExcludedDimensionLimit) = 0;
		virtual int                     GetReferenceCount() const = 0;
	};
}