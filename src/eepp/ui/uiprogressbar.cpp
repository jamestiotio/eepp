#include <eepp/ui/uiprogressbar.hpp>
#include <eepp/ui/uithememanager.hpp>
#include <eepp/scene/scenenode.hpp>
#include <eepp/graphics/globaltextureatlas.hpp>

namespace EE { namespace UI {

UIProgressBar *UIProgressBar::New() {
	return eeNew( UIProgressBar, () );
}

UIProgressBar::UIProgressBar() :
	UIWidget( "progressbar" ),
	mProgress( 0.f ),
	mTotalSteps( 100.f ),
	mFillerSkin( NULL )
{
	subscribeScheduledUpdate();

	setFlags( UI_AUTO_PADDING | UI_AUTO_SIZE );

	mTextBox = UITextView::NewWithTag( "progressbar::text" );
	mTextBox->setHorizontalAlign( UI_HALIGN_CENTER );
	mTextBox->setParent( this );
	mTextBox->setEnabled( false );

	updateTextBox();

	applyDefaultTheme();
}

UIProgressBar::~UIProgressBar() {
}

Uint32 UIProgressBar::getType() const {
	return UI_TYPE_PROGRESSBAR;
}

bool UIProgressBar::isType( const Uint32& type ) const {
	return UIProgressBar::getType() == type ? true : UIWidget::isType( type );
}

void UIProgressBar::draw() {
	UINode::draw();

	if ( NULL == mFillerSkin )
		return;

	Rectf fillerPadding = PixelDensity::dpToPx( mStyleConfig.FillerPadding );

	Float Height = (Float)mSize.getHeight();

	if ( !mStyleConfig.VerticalExpand )
		Height = (Float)mFillerSkin->getSize().getHeight();

	if ( Height > mSize.getHeight() )
		Height = mSize.getHeight();

	Sizef fSize( ( ( mSize.getWidth() - fillerPadding.Left - fillerPadding.Right ) * mProgress ) / mTotalSteps, Height - fillerPadding.Top - fillerPadding.Bottom );
	Sizei rSize( PixelDensity::dpToPxI( mFillerSkin->getSize() ) );
	Sizei numTiles( (Int32)eeceil( (Float)fSize.getWidth() / (Float)rSize.getWidth() + 2 ),
				(Int32)eeceil( (Float)fSize.getHeight() / (Float)rSize.getHeight() ) + 2 );

	clipSmartEnable( mScreenPos.x + fillerPadding.Left, mScreenPos.y + fillerPadding.Top, fSize.getWidth(), fSize.getHeight() );

	for ( int y = -1; y < numTiles.y; y++ ) {
		for ( int x = -1; x < numTiles.x; x++ ) {
			mFillerSkin->draw( Vector2f( (Int32)mOffset.x + mScreenPosi.x + fillerPadding.Left + x * rSize.getWidth(), mOffset.y + mScreenPosi.y + fillerPadding.Top + y * rSize.getHeight() ), Sizef( rSize.getWidth(), rSize.getHeight() ) );
		}
	}

	clipSmartDisable();
}

void UIProgressBar::scheduledUpdate( const Time& time ) {
	if ( NULL == mFillerSkin )
		return;

	Vector2f offset( mOffset );

	mOffset += mStyleConfig.MovementSpeed * (Float)( time.asSeconds() );

	Sizei rSize( PixelDensity::dpToPxI( mFillerSkin->getSize() ) );

	if ( mOffset.x > rSize.getWidth() || mOffset.x < -rSize.getWidth() )
		mOffset.x = 0.f;

	if ( mOffset.y > rSize.getHeight() || mOffset.y < -rSize.getHeight() )
		mOffset.y = 0.f;

	if ( offset != mOffset )
		invalidateDraw();
}

void UIProgressBar::setTheme( UITheme * Theme ) {
	UIWidget::setTheme( Theme );
	setThemeSkin( Theme, "progressbar" );

	mFillerSkin = Theme->getSkin( "progressbar_filler" );

	if ( mFillerSkin ) {
		if ( mFlags & UI_AUTO_PADDING ) {
			Float meH = (Float)getSkinSize().getHeight();
			Float otH = (Float)mFillerSkin->getSize().getHeight();
			Float res = Math::roundUp( ( meH - otH ) * 0.5f );
			mStyleConfig.FillerPadding = Rectf( res, res, res, res );
		}
	}

	onThemeLoaded();
}

void UIProgressBar::onThemeLoaded() {
	mMinControlSize.x = eemax( mMinControlSize.x, getSkinSize().getWidth() );
	mMinControlSize.y = eemax( mMinControlSize.y, getSkinSize().getHeight() );

	if ( mFlags & UI_AUTO_SIZE ) {
		setSize( mDpSize.x, getSkinSize().getHeight() );
	}

	UIWidget::onThemeLoaded();
}

Uint32 UIProgressBar::onValueChange() {
	UINode::onValueChange();

	onSizeChange();

	return 1;
}

void UIProgressBar::onSizeChange() {
	updateTextBox();
}

void UIProgressBar::setProgress( Float Val ) {
	mProgress = Val;

	onValueChange();
	updateTextBox();
}

const Float& UIProgressBar::getProgress() const {
	return mProgress;
}

void UIProgressBar::setTotalSteps( const Float& Steps ) {
	mTotalSteps = Steps;

	onSizeChange();
	updateTextBox();
}

const Float& UIProgressBar::getTotalSteps() const {
	return mTotalSteps;
}

void UIProgressBar::setMovementSpeed( const Vector2f& Speed ) {
	mStyleConfig.MovementSpeed = Speed;
}

const Vector2f& UIProgressBar::getMovementSpeed() const {
	return mStyleConfig.MovementSpeed;
}

void UIProgressBar::setVerticalExpand( const bool& VerticalExpand ) {
	if ( VerticalExpand != mStyleConfig.VerticalExpand ) {
		mStyleConfig.VerticalExpand = VerticalExpand;

		onSizeChange();
	}
}

const bool& UIProgressBar::getVerticalExpand() const {
	return mStyleConfig.VerticalExpand;
}

void UIProgressBar::setFillerPadding( const Rectf& padding ) {
	mStyleConfig.FillerPadding = padding;

	onPositionChange();
	onSizeChange();
}

const Rectf& UIProgressBar::getFillerPadding() const {
	return mStyleConfig.FillerPadding;
}

void UIProgressBar::setDisplayPercent( const bool& DisplayPercent ) {
	mStyleConfig.DisplayPercent = DisplayPercent;

	updateTextBox();
}

const bool& UIProgressBar::getDisplayPercent() const {
	return mStyleConfig.DisplayPercent;
}

void UIProgressBar::updateTextBox() {
	mTextBox->setVisible( mStyleConfig.DisplayPercent );
	mTextBox->setText( String::toStr( (Int32)( ( mProgress / mTotalSteps ) * 100.f ) ) + "%" );
	mTextBox->center();
}

UITextView * UIProgressBar::getTextBox() const {
	return mTextBox;
}

bool UIProgressBar::setAttribute( const NodeAttribute& attribute, const Uint32& state ) {
	const std::string& name = attribute.getName();

	if ( "total-steps" == name || "totalsteps" == name ) {
		setTotalSteps( attribute.asFloat() );
	} else if ( "progress" == name ) {
		setProgress( attribute.asFloat() );
	} else if ( "vertical-expand" == name || "verticalexpand" == name ) {
		setVerticalExpand( attribute.asBool() );
	} else if ( "display-percent" == name || "displaypercent" == name ) {
		setDisplayPercent( attribute.asBool() );
	} else if ( "filler-padding" == name || "fillerpadding" == name ) {
		Float val = attribute.asDpDimension();
		setFillerPadding( Rectf( val, val, val, val ) );
	} else if ( "filler-padding-left" == name || "fillerpaddingleft" == name ) {
		setFillerPadding( Rectf( attribute.asDpDimension(), mStyleConfig.FillerPadding.Top, mStyleConfig.FillerPadding.Right, mStyleConfig.FillerPadding.Bottom ) );
	} else if ( "filler-padding-right" == name || "fillerpaddingright" == name ) {
		setFillerPadding( Rectf( mStyleConfig.FillerPadding.Left, mStyleConfig.FillerPadding.Top, attribute.asDpDimension(), mStyleConfig.FillerPadding.Bottom ) );
	} else if ( "filler-padding-top" == name || "fillerpaddingtop" == name ) {
		setFillerPadding( Rectf( mStyleConfig.FillerPadding.Left, attribute.asDpDimension(), mStyleConfig.FillerPadding.Right, mStyleConfig.FillerPadding.Bottom ) );
	} else if ( "filler-padding-bottom" == name || "fillerpaddingbottom" == name ) {
		setFillerPadding( Rectf( mStyleConfig.FillerPadding.Left, mStyleConfig.FillerPadding.Top, mStyleConfig.FillerPadding.Right, attribute.asDpDimension() ) );
	} else if ( "movement-speed" == name || "movementspeed" == name ) {
		setMovementSpeed( attribute.asVector2f() );
	} else {
		return UIWidget::setAttribute( attribute, state );
	}

	return true;
}

const UIProgressBar::StyleConfig & UIProgressBar::getStyleConfig() const {
	return mStyleConfig;
}

void UIProgressBar::onAlphaChange() {
	UINode::onAlphaChange();

	mTextBox->setAlpha( mAlpha );
}

}}
