#ifndef EE_GAMINGCTILEMAP_HPP
#define EE_GAMINGCTILEMAP_HPP

#include <eepp/gaming/base.hpp>

#include <eepp/gaming/gameobject.hpp>
#include <eepp/gaming/gameobjectobject.hpp>
#include <eepp/gaming/maplight.hpp>
#include <eepp/gaming/maplightmanager.hpp>
#include <eepp/gaming/maplayer.hpp>

#include <eepp/window/input.hpp>
#include <eepp/window/engine.hpp>
#include <eepp/window/window.hpp>
using namespace EE::Window;

#include <eepp/graphics/texture.hpp>
using namespace EE::Graphics;

namespace EE { namespace Gaming {

namespace Private { class UIMapNew; }

#define EE_MAP_LAYER_UNKNOWN eeINDEX_NOT_FOUND
#define EE_MAP_MAGIC ( ( 'E' << 0 ) | ( 'E' << 8 ) | ( 'M' << 16 ) | ( 'P' << 24 ) )

class EE_API TileMap {
	public:
		typedef std::map<std::string, std::string>	PropertiesMap;
		typedef std::list<std::string>				GOTypesList;		//! Special object types used in this map
		typedef cb::Callback4< GameObject *, const Uint32&, const Uint32&, MapLayer *, const Uint32&>		CreateGOCb;
		typedef cb::Callback0<void>	MapDrawCb;
		typedef cb::Callback0<void> MapUpdateCb;

		TileMap();

		virtual ~TileMap();

		virtual void create( Sizei Size, Uint32 MaxLayers, Sizei TileSize, Uint32 Flags = 0, Sizei viewSize = Sizei( 800, 600 ), EE::Window::Window * Window = NULL );

		virtual MapLayer * addLayer( Uint32 Type, Uint32 flags, std::string name );

		virtual MapLayer * getLayer( Uint32 index );

		virtual Uint32 getLayerIndex( MapLayer * Layer );

		virtual MapLayer * getLayerByHash( Uint32 hash );

		virtual MapLayer * getLayer( const std::string& name );

		virtual bool load( const std::string& path );

		virtual bool loadFromStream( IOStream& IOS );

		virtual bool loadFromPack( Pack * Pack, const std::string& FilePackPath );

		virtual bool loadFromMemory( const char * Data, const Uint32& DataSize );

		virtual void save( const std::string& path );

		virtual void saveToStream( IOStream& IOS );

		virtual void draw();

		virtual void update();

		const Sizei& getTileSize() const;

		const Sizei& getSize() const;

		const Uint32& getLayerCount() const;

		const Uint32& getMaxLayers() const;

		const Sizei& getViewSize() const;

		void setViewSize( const Sizei& viewSize );

		const Vector2f& getOffset() const;

		void setOffset( const Vector2f& offset );

		const Vector2i& getPosition() const;

		void setPosition( const Vector2i& position );

		void move( const Vector2f& offset );

		void move( const Float& offsetx, const Float& offsety );

		const Vector2i& getStartTile() const;

		const Vector2i& getEndTile() const;

		const Uint32& getFlags() const;

		bool getClampBorders() const;

		void setClampBorders( const bool& clamp );

		bool getClipedArea() const;

		void setClipedArea( const bool& clip );

		Uint32 getDrawGrid() const;

		void setDrawGrid( const bool& draw );

		Uint32 getShowBlocked() const;

		void setShowBlocked( const bool& show );

		Uint32 getDrawBackground() const;

		void setDrawBackground( const bool& draw );

		Uint32 getDrawTileOver() const;

		void setDrawTileOver( const bool& draw );

		bool getLightsEnabled();

		void setLightsEnabled( const bool& enabled );

		bool getLightsByVertex();

		void reset();

		bool moveLayerUp( MapLayer * Layer );

		bool moveLayerDown( MapLayer * Layer );

		bool removeLayer( MapLayer * Layer );

		const Vector2i& getMouseTilePos() const;

		Vector2i getMouseTilePosCoords();

		Vector2i getTileCoords( const Vector2i& TilePos );

		const Vector2i& getRealMouseTilePos() const;

		const Vector2i& getMouseMapPos() const;

		Vector2f getMouseMapPosf() const;

		const Sizei& getTotalSize() const;

		void addProperty( std::string Text, std::string Value );

		void editProperty( std::string Text, std::string Value );

		void removeProperty( std::string Text );

		void clearProperties();

		PropertiesMap& getProperties();

		void addVirtualObjectType( const std::string& name );

		void removeVirtualObjectType( const std::string& name );

		void clearVirtualObjectTypes();

		GOTypesList& getVirtualObjectTypes();

		void setCreateGameObjectCallback( const CreateGOCb& Cb );

		const std::string& getPath() const;

		void setBaseColor( const ColorA& color );

		const ColorA& getBaseColor() const;

		const eeAABB& getViewAreaAABB() const;

		MapLightManager * getLightManager() const;

		/** Tiles to add or subtract from the real values of StartTile and EndTile ( so it will loop over more/less tiles than the required to render every tile on screen ). */
		void setExtraTiles( const Vector2i& extra );

		const Vector2i& getExtraTiles() const;

		void setDrawCallback( MapDrawCb Cb );

		void setUpdateCallback( MapUpdateCb Cb );

		Texture * getBlankTileTexture();

		bool isTileBlocked( const Vector2i& TilePos );

		void setData( void * value );

		void * getData() const;

		const bool& isMouseOver() const;

		GameObject * IsTypeInTilePos( const Uint32& Type, const Vector2i& TilePos );

		const Uint8& getBackAlpha() const;

		void setBackAlpha( const Uint8& alpha );

		const ColorA& getBackColor() const;

		void setBackColor( const ColorA& col );

		const Float& getScale() const;

		void setScale( const Float& scale );

		Vector2i getMaxOffset();

		Uint32 getNewObjectId();

		GameObjectPolyData& getPolyObjData( Uint32 Id );

		void forceHeadersOnLoad( Sizei mapSize, Sizei tileSize, Uint32 numLayers, Uint32 flags );

		void disableForcedHeaders();

		void setGridLinesColor( const ColorA& Col );

		const ColorA& setGridLinesColor() const;
	protected:
		friend class EE::Gaming::Private::UIMapNew;

		class ForcedHeaders
		{
			public:
				ForcedHeaders( Sizei mapSize, Sizei tileSize, Uint32 numLayers, Uint32 flags ) :
					MapSize( mapSize ),
					TileSize( tileSize ),
					NumLayers( numLayers ),
					Flags( flags )
				{}

				Sizei MapSize;
				Sizei TileSize;
				Uint32 NumLayers;
				Uint32 Flags;
		};

		typedef std::map<Uint32, GameObjectPolyData> PolyObjMap;

		EE::Window::Window *		mWindow;
		MapLayer**		mLayers;
		Uint32			mFlags;
		Uint32			mMaxLayers;
		Uint32			mLayerCount;
		Sizei			mSize;
		Sizei			mPixelSize;
		Sizei			mTileSize;
		Sizei			mViewSize;
		Vector2f		mOffset;
		Vector2i		mScreenPos;
		Vector2i		mStartTile;
		Vector2i		mEndTile;
		Vector2i		mExtraTiles;
		Vector2i		mMouseOverTile;
		Vector2i		mMouseOverTileFinal;
		Vector2i		mMouseMapPos;
		ColorA		mBaseColor;
		PropertiesMap	mProperties;
		GOTypesList		mObjTypes;
		CreateGOCb		mCreateGOCb;
		Texture *		mTileTex;
		eeAABB			mScreenAABB;
		MapLightManager *	mLightManager;
		MapDrawCb		mDrawCb;
		MapUpdateCb		mUpdateCb;
		void *			mData;
		ColorA		mTileOverColor;
		ColorA		mBackColor;
		ColorA		mGridLinesColor;
		Uint8			mBackAlpha;
		bool			mMouseOver;
		std::string		mPath;
		Float			mScale;
		Vector2f		mOffscale;
		Uint32			mLastObjId;
		PolyObjMap		mPolyObjs;
		ForcedHeaders*	mForcedHeaders;

		virtual GameObject *	CreateGameObject( const Uint32& Type, const Uint32& Flags, MapLayer * Layer, const Uint32& DataId = 0 );

		void			CalcTilesClip();

		void			Clamp();

		void			GetMouseOverTile();

		void			GridDraw();

		void			MouseOverDraw();

		void			DeleteLayers();

		std::vector<std::string> GetTextureAtlases();

		void			CreateEmptyTile();

		void			UpdateScreenAABB();

		void			CreateLightManager();

		virtual void	OnMapLoaded();
};

}}

#endif
