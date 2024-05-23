#include "ruby_pch.h"

#undef INFINITE
#include <msdf-atlas-gen/msdf-atlas-gen.h>
#include <msdf-atlas-gen/FontGeometry.h>
#include <msdf-atlas-gen/GlyphGeometry.h>

#include "Font.h"
#include "Ruby/Main/App.h"

namespace Ruby {

	static msdfgen::FreetypeHandle* s_Lib{ nullptr };

    struct FontData
    {
        std::vector<msdf_atlas::GlyphGeometry> Glyphs;
        msdf_atlas::FontGeometry FontGeometry;
    };


	Font::Font(const std::string& filepath)
	{
		RB_ASSERT_RET_VOID(s_Lib, "FT Library not initialized, maybe you forgot to call Ruby::Font::init()?");
        
        // Load font file
        msdfgen::FontHandle* font = msdfgen::loadFont(s_Lib, filepath.c_str());
        RB_ASSERT_RET_VOID(font, "Unable to load font at path \'%s\'.", filepath.c_str());

        m_Data = new FontData();
        m_Data->FontGeometry = msdf_atlas::FontGeometry(&m_Data->Glyphs);
        int loadedChars = m_Data->FontGeometry.loadCharset(font, 1.0, msdf_atlas::Charset::ASCII);
        RB_INFO("Loaded %d out of %llu characters from font.", loadedChars, msdf_atlas::Charset::ASCII.size());

        constexpr double maxCornerAngle = 3.0;
        for (msdf_atlas::GlyphGeometry& glyph : m_Data->Glyphs)
            glyph.edgeColoring(&msdfgen::edgeColoringInkTrap, maxCornerAngle, 0ull);


        msdf_atlas::TightAtlasPacker packer;

        packer.setDimensionsConstraint(msdf_atlas::DimensionsConstraint::POWER_OF_TWO_SQUARE);
        packer.setScale(40.0);
        packer.setPixelRange(2.0);
        packer.setMiterLimit(1.0);

        int failure = packer.pack(m_Data->Glyphs.data(), (int)m_Data->Glyphs.size());
        RB_ASSERT(!failure, "Failed to pack glyphs into atlas texture.");

        
        int width = 0, height = 0;
        packer.getDimensions(width, height);


        msdf_atlas::ImmediateAtlasGenerator<float, 3, 
                                            &msdf_atlas::msdfGenerator, 
                                            msdf_atlas::BitmapAtlasStorage<uint8_t, 3>> generator(width, height);
       
        msdf_atlas::GeneratorAttributes attributes;
        // Support fonts with self-overlapping glyphs.
        attributes.config.overlapSupport = true;
        attributes.scanlinePass = true;
        generator.setAttributes(attributes);
        generator.setThreadCount(4);

        generator.generate(m_Data->Glyphs.data(), (int)m_Data->Glyphs.size());
        // The atlas bitmap can now be retrieved via atlasStorage as a BitmapConstRef.
        msdfgen::BitmapConstRef<uint8_t, 3> bitmapRef = generator.atlasStorage();
        TextureSpec spec;
        spec.Width = width;
        spec.Height = height;
        spec.Format = PixelFormat::RGB8;
        m_AtlasTexture = Texture::createTexture((void*)bitmapRef.pixels, spec);
        
        // Cleanup
        msdfgen::destroyFont(font);
	}

    Font::~Font()
    {
        delete m_Data;
    }

    void Font::init()
    {
        s_Lib = msdfgen::initializeFreetype();
        RB_ASSERT_CRITICAL(s_Lib, "MSDFGen Error: Freetype failed to initialize.");
    }

    void Font::deInit()
    {
        msdfgen::deinitializeFreetype(s_Lib);
    }

	
}