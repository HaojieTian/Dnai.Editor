#ifndef ICOMMAND_H
#define ICOMMAND_H

#include <QString>

namespace duly_gui
{
	namespace commands
	{
		class ICommand
		{
		public:
			virtual ~ICommand() = default;

			/**
			* \brief Execute the command
			*/
			virtual void execute() const = 0;

			/**
			* \brief Execute the command
			*/
			virtual void executeSave() = 0;

			/**
			* \brief Reverse the command
			*/
			virtual void unExcute() const = 0;

			virtual QString infos() const = 0;

			virtual bool isSave() const = 0;
		};
	}
}


#endif // ICOMMAND_H