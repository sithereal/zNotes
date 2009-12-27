#include "settings.h"

#include <QObject>
#include <QtDebug>

/*
  Settings loading...
*/
Settings::Settings() : config("pDev", "zNotes")
{
	if(config.allKeys().size()>0) //if exist - reading settings
	{
		NotesPath = config.value("NotesPath").toString();
		LastNote = config.value("LastNote").toString();
		HideStart = config.value("HideStart").toBool();
		//
		DialogGeometry = config.value("DialogGeometry").toByteArray();
		DialogState = config.value("DialogState").toByteArray();
		//
		//HideToolbar = config.value("HideToolbar").toBool();
		HideFrame = config.value("HideFrame").toBool();
		StayTop = config.value("StayTop").toBool();
		//
		NoteFont.fromString(config.value("NoteFont").toString());
		//
		int ScriptCount = config.value("ComandCount").toInt();
		for(int i=0; i<ScriptCount; ++i)
		{
			smodel.append(
				config.value(QString("ComandName%1").arg(i)).toString(),
				config.value(QString("ComandFile%1").arg(i)).toString(),
				config.value(QString("ComandIcon%1").arg(i)).toString());
		}
		ScriptShowOutput = config.value("ScriptShowOutput").toBool();
		ScriptCopyOutput = config.value("ScriptCopyOutput").toBool();
		//
		///TODO: remove this code, when in version 0.4.1:
		//
		if(config.contains("Toolbar/itemCount"))
		{
			tb_items.resize(config.value("Toolbar/itemCount").toInt());
			for(int i=itemAdd; i<itemMax; ++i)
			{
				int pos = config.value(getItemName(i), tb_items.size()).toInt();
				if(pos<tb_items.size()) tb_items[pos] = i;
			}
		}
		else
		{
			bool old_settings_exist = (
					config.contains("tbHideEdit") ||
					config.contains("tbHideMove") ||
					config.contains("tbHideCopy") ||
					config.contains("tbHideSetup") ||
					config.contains("tbHideRun") ||
					config.contains("tbHideExit") );
			if(old_settings_exist) //converting old toolbar's settings
			{
				if(!config.value("tbHideEdit").toBool())
				{
					tb_items.append(itemAdd);
					tb_items.append(itemRemove);
					tb_items.append(itemRename);
					tb_items.append(itemSeparator);
				}
				if(!config.value("tbHideMove").toBool())
				{
					tb_items.append(itemPrev);
					tb_items.append(itemNext);
					tb_items.append(itemSeparator);
				}
				if(!config.value("tbHideCopy").toBool())
				{
					tb_items.append(itemCopy);
					tb_items.append(itemSeparator);
				}
				if(!config.value("tbHideSetup").toBool())
				{
					tb_items.append(itemSetup);
					tb_items.append(itemInfo);
					tb_items.append(itemSeparator);
				}
				if(!config.value("tbHideRun").toBool())
				{
					tb_items.append(itemRun);
					tb_items.append(itemSearch);
					tb_items.append(itemSeparator);
				}
				if(!config.value("tbHideExit").toBool()) tb_items.append(itemExit);
			}
			else //default toolbar's settings
			{
				tb_items.append(itemAdd);
				tb_items.append(itemRemove);
				tb_items.append(itemRename);
				tb_items.append(itemSeparator);
				tb_items.append(itemPrev);
				tb_items.append(itemNext);
				tb_items.append(itemSeparator);
				tb_items.append(itemCopy);
				tb_items.append(itemSeparator);
				tb_items.append(itemSetup);
				tb_items.append(itemInfo);
				tb_items.append(itemSeparator);
				tb_items.append(itemRun);
				tb_items.append(itemSearch);
				tb_items.append(itemSeparator);
				tb_items.append(itemExit);
			}
		}
	}
	else //if settings don't exist - setup default settings
	{
		tb_items.append(itemAdd);
		tb_items.append(itemRemove);
		tb_items.append(itemRename);
		tb_items.append(itemSeparator);
		tb_items.append(itemPrev);
		tb_items.append(itemNext);
		tb_items.append(itemSeparator);
		tb_items.append(itemCopy);
		tb_items.append(itemSeparator);
		tb_items.append(itemSetup);
		tb_items.append(itemInfo);
		tb_items.append(itemSeparator);
		tb_items.append(itemRun);
		tb_items.append(itemSearch);
		tb_items.append(itemSeparator);
		tb_items.append(itemExit);
	}
}

/*
  Saving notes's path
*/
void Settings::setNotesPath(const QString& path)
{
	if(NotesPath != path)
	{
		NotesPath = path;
		config.setValue("NotesPath", NotesPath);
		emit NotesPathChanged();
	}
}

/*
  Saving title of last note
*/
void Settings::setLastNote(const QString& note_name)
{
	if(LastNote != note_name)
	{
		LastNote = note_name;
		config.setValue("LastNote", LastNote);
	}
}

/*
  Saving option (hiding dialog on start)
*/
void Settings::setHideStart(bool hide)
{
	if(HideStart != hide)
	{
		HideStart = hide;
		config.setValue("HideStart", HideStart);
	}
}

/*
  Saving option (toolbar's showing)
*/
//void Settings::setHideToolbar(bool Hide, bool send_signal)
//{
//	if(HideToolbar != Hide)
//	{
//		HideToolbar = Hide;
//		config.setValue("HideToolbar", HideToolbar);
//		if(send_signal) emit ToolbarVisChanged();
//	}
//}
//
/*
  Saving option (hiding window decoration)
*/
void Settings::setHideFrame(bool Hide)
{
	if(HideFrame != Hide)
	{
		HideFrame = Hide;
		config.setValue("HideFrame", HideFrame);
		emit WindowStateChanged();
	}
}

/*
  Saving option (staying on top)
*/
void Settings::setStayTop(bool top)
{
	if(StayTop != top)
	{
		StayTop = top;
		config.setValue("StayTop", StayTop);
		emit WindowStateChanged();
	}
}

/*
  Saving dialog's params
*/
void Settings::setDialogGeometry(const QByteArray& g)
{
	DialogGeometry = g;
	config.setValue("DialogGeometry", DialogGeometry);
}
void Settings::setDialogState(const QByteArray& g)
{
	DialogState = g;
	config.setValue("DialogState", DialogState);
}

/*
  Saving notes's font
*/
void Settings::setNoteFont(const QFont& f)
{
	if(NoteFont != f)
	{
		NoteFont = f;
		config.setValue("NoteFont", NoteFont.toString());
		emit NoteFontChanged();
	}
}

/*
  Saving script's options
*/
void Settings::setScriptShowOutput(bool sso)
{
	if(ScriptShowOutput != sso)
	{
		ScriptShowOutput = sso;
		config.setValue("ScriptShowOutput", ScriptShowOutput);
	}
}
void Settings::setScriptCopyOutput(bool sco)
{
	if(ScriptCopyOutput != sco)
	{
		ScriptCopyOutput = sco;
		config.setValue("ScriptCopyOutput", ScriptCopyOutput);
	}
}

/*
  Saving scripts
*/
void Settings::setScripts()
{
	config.setValue("ComandCount", smodel.rowCount());
	for(int i=0; i<smodel.rowCount(); ++i)
	{
		config.setValue(QString("ComandName%1").arg(i), smodel.getName(i));
		config.setValue(QString("ComandFile%1").arg(i), smodel.getFile(i));
		config.setValue(QString("ComandIcon%1").arg(i), smodel.getIcon(i));
	}
}

/*
  Saving toolbar's items
*/
void Settings::setToolbarItems(const QVector<int>& v)
{
	if(v==tb_items) return;
	//removing old settings
	for(int i=0; i<tb_items.size(); ++i) if(tb_items[i]!=itemSeparator)
	{
		config.remove(getItemName(tb_items[i])); //dirty hack =(
	}
	tb_items = v;
	//saving settings
	config.setValue("Toolbar/itemCount", tb_items.size());
	for(int i=0; i<tb_items.size(); ++i) if(tb_items[i]!=itemSeparator)
	{
		config.setValue(getItemName(tb_items[i]), i);
	}
	emit ToolbarItemsChanged();
}
