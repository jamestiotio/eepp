#ifndef EE_UI_DOC_FOLDRANGESERVICE_HPP
#define EE_UI_DOC_FOLDRANGESERVICE_HPP

#include <eepp/config.hpp>
#include <eepp/system/mutex.hpp>
#include <eepp/ui/doc/textrange.hpp>
#include <functional>
#include <optional>
#include <unordered_map>

using namespace EE::System;

namespace EE { namespace UI { namespace Doc {

class TextDocument;

class EE_API FoldRangeServive {
  public:
	using FoldRangeProvider = std::function<bool( TextDocument*, bool /* requestFolds */ )>;

	FoldRangeServive( TextDocument* doc );

	bool canFold() const;

	void findRegions();

	void clear();

	bool empty();

	std::optional<TextRange> find( Int64 docIdx );

	void addFoldRegion( TextRange region );

	bool isFoldingRegionInLine( Int64 docIdx );

	void removeFoldedRegion( const TextRange& region );

	void shiftFoldingRegions( Int64 fromLine, Int64 numLines );

	void setFoldingRegions( std::vector<TextRange> regions );

	const FoldRangeProvider& getProvider() const;

	bool hasProvider() const;

	void setProvider( const FoldRangeProvider& provider );

	bool isEnabled() const;

	void setEnabled( bool enabled );

  protected:
	TextDocument* mDoc;
	std::unordered_map<Int64, TextRange> mFoldingRegions;
	FoldRangeProvider mProvider{ nullptr };
	Mutex mMutex;
	bool mEnabled{ true };
};

}}} // namespace EE::UI::Doc

#endif // EE_UI_DOC_FOLDRANGESERVICE_HPP
