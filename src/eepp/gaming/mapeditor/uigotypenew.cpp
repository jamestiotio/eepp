#include <eepp/gaming/mapeditor/uigotypenew.hpp>
#include <eepp/ui/uitextinput.hpp>

namespace EE { namespace Gaming { namespace Private {

UIGOTypeNew::UIGOTypeNew( cb::Callback2<void, std::string, Uint32> Cb ) :
	mUITheme( NULL ),
	mUIWindow( NULL ),
	mUIInput( NULL ),
	mCb( Cb )
{
	mUITheme		= UIThemeManager::instance()->defaultTheme();

	if ( NULL == mUITheme )
		return;

	mUIWindow	= mUITheme->createWindow( NULL, Sizei( 278, 114 ), Vector2i(), UI_CONTROL_DEFAULT_FLAGS_CENTERED, UI_WIN_DEFAULT_FLAGS | UI_WIN_MODAL, Sizei( 278, 114 ) );
	mUIWindow->addEventListener( UIEvent::EventOnWindowClose, cb::Make1( this, &UIGOTypeNew::WindowClose ) );
	mUIWindow->title( "Add GameObject Type" );

	Int32 InitialY		= 16;
	Int32 DistFromTitle	= 18;

	UITextBox * Txt = mUITheme->createTextBox( "GameObject Type Name", mUIWindow->getContainer(), Sizei(), Vector2i( 16, InitialY ), UI_CONTROL_DEFAULT_FLAGS | UI_DRAW_SHADOW | UI_AUTO_SIZE );

	mUIInput = mUITheme->createTextInput( mUIWindow->getContainer(), Sizei( 120, 22 ), Vector2i( Txt->position().x + DistFromTitle, Txt->position().y + DistFromTitle ), UI_CONTROL_DEFAULT_FLAGS | UI_CLIP_ENABLE | UI_AUTO_PADDING | UI_AUTO_SIZE, true, 64 );

	UIPushButton * OKButton = mUITheme->createPushButton( mUIWindow->getContainer(), Sizei( 80, 22 ), Vector2i(), UI_CONTROL_DEFAULT_FLAGS_CENTERED | UI_AUTO_SIZE, mUITheme->getIconByName( "add" ) );
	OKButton->position( mUIWindow->getContainer()->size().getWidth() - OKButton->size().getWidth() - 4, mUIWindow->getContainer()->size().getHeight() - OKButton->size().getHeight() - 4 );
	OKButton->addEventListener( UIEvent::EventMouseClick, cb::Make1( this, &UIGOTypeNew::OKClick ) );
	mUIInput->addEventListener( UIEvent::EventOnPressEnter, cb::Make1( this, &UIGOTypeNew::OKClick ) );

	OKButton->text( "Add" );

	UIPushButton * CancelButton = mUITheme->createPushButton( mUIWindow->getContainer(), OKButton->size(), Vector2i( OKButton->position().x - OKButton->size().getWidth() - 4, OKButton->position().y ), UI_CONTROL_DEFAULT_FLAGS_CENTERED | UI_AUTO_SIZE, mUITheme->getIconByName( "cancel" ) );
	CancelButton->addEventListener( UIEvent::EventMouseClick, cb::Make1( this, &UIGOTypeNew::CancelClick ) );
	CancelButton->text( "Cancel" );

	mUIWindow->center();
	mUIWindow->show();

	mUIInput->setFocus();
}

UIGOTypeNew::~UIGOTypeNew() {

}

void UIGOTypeNew::OKClick( const UIEvent * Event ) {
	if ( mUIInput->text().size() ) {
		if ( mCb.IsSet() )
			mCb( mUIInput->text().toUtf8(), String::hash( mUIInput->text().toUtf8() ) );
	}

	mUIWindow->CloseWindow();
}

void UIGOTypeNew::CancelClick( const UIEvent * Event ) {
	mUIWindow->CloseWindow();
}

void UIGOTypeNew::WindowClose( const UIEvent * Event ) {
	eeDelete( this );
}

}}}
