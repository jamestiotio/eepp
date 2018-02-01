#include <eepp/ui/uiimage.hpp>
#include <eepp/graphics/drawable.hpp>
#include <eepp/graphics/sprite.hpp>
#include <eepp/graphics/drawablesearcher.hpp>
#include <pugixml/pugixml.hpp>

namespace EE { namespace UI {

UIImage * UIImage::New() {
	return eeNew( UIImage, () );
}

UIImage::UIImage() :
	UIWidget(),
	mScaleType( 0 ),
	mDrawable( NULL ),
	mColor(),
	mAlignOffset(0,0),
	mDrawableOwner(false)
{
	mFlags |= UI_AUTO_SIZE;

	onAutoSize();
}

UIImage::~UIImage() {
	safeDeleteDrawable();
}

Uint32 UIImage::getType() const {
	return UI_TYPE_IMAGE;
}

bool UIImage::isType( const Uint32& type ) const {
	return UIImage::getType() == type ? true : UIWidget::isType( type );
}

UIImage * UIImage::setDrawable( Drawable * drawable ) {
	safeDeleteDrawable();

	mDrawable = drawable;

	onAutoSize();

	if ( NULL != mDrawable && mSize.x == 0 && mSize.y == 0 ) {
		setSize( mDrawable->getSize() );
	}

	autoAlign();

	notifyLayoutAttrChange();

	invalidateDraw();

	return this;
}

void UIImage::onAutoSize() {
	if ( ( mFlags & UI_AUTO_SIZE ) && Sizef::Zero == mSize ) {
		if ( NULL != mDrawable ) {
			setSize( mDrawable->getSize() );
		}
	}
}

void UIImage::calcDestSize() {
	if ( mScaleType == UIScaleType::Expand ) {
		mDestSize = Sizef( mRealSize.x, mRealSize.y );
	} else if ( mScaleType == UIScaleType::FitInside ) {
		if ( NULL == mDrawable)
			return;

		Sizef pxSize( PixelDensity::dpToPx( mDrawable->getSize() ) );
		Float Scale1 = mRealSize.x / pxSize.x;
		Float Scale2 = mRealSize.y / pxSize.y;

		if ( Scale1 < 1 || Scale2 < 1 ) {
			if ( Scale2 < Scale1 )
				Scale1 = Scale2;

			mDestSize = Sizef( pxSize.x * Scale1, pxSize.y * Scale1 );
		} else {
			mDestSize = Sizef( pxSize.x, pxSize.y );
		}
	} else {
		if ( NULL == mDrawable)
			return;

		mDestSize = PixelDensity::dpToPx( mDrawable->getSize() );
	}

	mDestSize = mDestSize.floor();

	autoAlign();
}

void UIImage::draw() {
	UINode::draw();

	if ( mVisible ) {
		if ( NULL != mDrawable && 0.f != mAlpha ) {
			calcDestSize();

			mDrawable->setColor( mColor );
			mDrawable->draw( Vector2f( (Float)mScreenPosi.x + (int)mAlignOffset.x, (Float)mScreenPosi.y + (int)mAlignOffset.y ), mDestSize );
			mDrawable->clearColor();
		}
	}
}

void UIImage::setAlpha( const Float& alpha ) {
	UINode::setAlpha( alpha );
	mColor.a = (Uint8)alpha;
}

Drawable * UIImage::getDrawable() const {
	return mDrawable;
}

const Color& UIImage::getColor() const {
	return mColor;
}

UIImage * UIImage::setColor( const Color& col ) {
	mColor = col;
	setAlpha( col.a );
	return this;
}

void UIImage::autoAlign() {
	if ( NULL == mDrawable )
		return;

	if ( HAlignGet( mFlags ) == UI_HALIGN_CENTER ) {
		mAlignOffset.x = ( mRealSize.getWidth() - mDestSize.x ) / 2;
	} else if ( fontHAlignGet( mFlags ) == UI_HALIGN_RIGHT ) {
		mAlignOffset.x =  mRealSize.getWidth() - mDestSize.x;
	} else {
		mAlignOffset.x = 0;
	}

	if ( VAlignGet( mFlags ) == UI_VALIGN_CENTER ) {
		mAlignOffset.y = ( mRealSize.getHeight() - mDestSize.y ) / 2;
	} else if ( fontVAlignGet( mFlags ) == UI_VALIGN_BOTTOM ) {
		mAlignOffset.y = mRealSize.getHeight() - mDestSize.y;
	} else {
		mAlignOffset.y = 0;
	}
}

void UIImage::safeDeleteDrawable() {
	if ( NULL != mDrawable && mDrawableOwner ) {
		if ( mDrawable->getDrawableType() == Drawable::SPRITE ) {
			Sprite * spr = reinterpret_cast<Sprite*>( mDrawable );
			eeSAFE_DELETE( spr );
		}

		mDrawableOwner = false;
	}
}

void UIImage::onSizeChange() {
	onAutoSize();
	calcDestSize();
	UIWidget::onSizeChange();
}

void UIImage::onAlignChange() {
	UIWidget::onAlignChange();
	onAutoSize();
	calcDestSize();
}

const Vector2f& UIImage::getAlignOffset() const {
	return mAlignOffset;
}

void UIImage::loadFromXmlNode(const pugi::xml_node & node) {
	beginPropertiesTransaction();

	UIWidget::loadFromXmlNode( node );

	for (pugi::xml_attribute_iterator ait = node.attributes_begin(); ait != node.attributes_end(); ++ait) {
		std::string name = ait->name();
		String::toLowerInPlace( name );

		if ( "src" == name ) {
			Drawable * res = NULL;

			if ( NULL != ( res = DrawableSearcher::searchByName( ait->as_string() ) ) ) {
				if ( res->getDrawableType() == Drawable::SPRITE )
					mDrawableOwner = true;

				setDrawable( res );
			}
		} else if ( "scaletype" == name ) {
			std::string val = ait->as_string();
			String::toLowerInPlace( val );

			if ( "expand" == val ) {
				setScaleType( UIScaleType::Expand );
			} else if ( "fit_inside" == val || "fitinside" == val ) {
				setScaleType( UIScaleType::FitInside );
			} else if ( "none" == val ) {
				setScaleType( UIScaleType::None );
			}
		}
	}

	endPropertiesTransaction();
}

Uint32 UIImage::getScaleType() const {
	return mScaleType;
}

UIImage * UIImage::setScaleType(const Uint32& scaleType) {
	mScaleType = scaleType;
	calcDestSize();
	invalidateDraw();
	return this;
}

}}
