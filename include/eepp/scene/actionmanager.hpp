#ifndef EE_SCENEACTIONMANAGER_HPP
#define EE_SCENEACTIONMANAGER_HPP

#include <list>
#include <eepp/config.hpp>
#include <eepp/system/time.hpp>
#include <eepp/system/mutex.hpp>
using namespace EE::System;

namespace EE { namespace Scene {

class Action;
class Node;

class EE_API ActionManager {
	public:
		static ActionManager * New();

		ActionManager();

		~ActionManager();

		void addAction( Action * action );

		Action * getActionByTag( const Uint32& tag );

		void removeActionByTag( const Uint32& tag );

		void removeAction( Action * action );

		void removeAllActionsFromTarget( Node * target );

		void removeActionsByTagFromTarget( Node * target, const Uint32& tag );

		std::vector<Action *> getActionsByTagFromTarget( Node * target, const Uint32& tag );

		void update( const Time& time );

		std::size_t count() const;

		bool isEmpty() const;

		void clear();
	protected:
		std::list<Action*> mActions;
		Mutex mMutex;
};

}}

#endif

