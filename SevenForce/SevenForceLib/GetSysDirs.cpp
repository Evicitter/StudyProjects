//Следующий пример использует SHGetFolderPath для поиска или создания директории, а затем создаёт в ней файл.

TCHAR szPath[MAX_PATH];

if(SUCCEEDED(SHGetFolderPath(NULL, 
                             CSIDL_PERSONAL|CSIDL_FLAG_CREATE,
                             NULL,
                             0,
                             szPath)))
{
    PathAppend(szPath, TEXT("New Doc.txt"));
    HANDLE hFile = CreateFile(szPath, ...);
}

// Функция SHGetFolderPath.

// Возвращает путь к определённой системной директории, тип которой указан в идентификаторе CSIDL.

// Синтаксис

// HRESULT SHGetFolderPath(      
    // HWND hwndOwner,
    // int nFolder,
    // HANDLE hToken,
    // DWORD dwFlags,
    // LPTSTR pszPath
// );
// Параметры

// hwndOwner
// Хэндл окна владельца. Обычно этот параметр равен NULL.
// nFolder
// Значение CSIDL которое идентифицирует директорию, путь которой мы хотим узнать. Если такой директории не существует, то можно её создать, скомбинировав CSIDL с CSIDL_FLAG_CREATE.
// hToken
// Обычно этот параметр устанавливается в NULL. Он нужен в многопользовательских системах, и работает начиная с Windows 2000, например, если необходимо узнать путь директории "Мои документы" определённого пользователя. Если указать в этом параметре -1, то можно узнать пути директорий для пользователя по умолчанию (Default User). "Пользователь по умолчанию, это профиль, на основе которого создаётся аккаунт каждого нового пользователя.
// dwFlags
// Флаги, указывающие на то, какую директорию необходимо вернуть функции в случае, если директория, указанная в CSIDL не существует или была перемещена или переименована.
// SHGFP_TYPE_CURRENT
// Возвращает текущую директорию.
// SHGFP_TYPE_DEFAULT
// Возвращает директорию поумолчанию.
// pszPath
// Указатель на строку (завершающуюся нулём) длиной MAX_PATH в которую будет помещён путь после вызова функции. В случае ошибки или если функция вернёт S_FALSE, эта строка будет пустой.
// Возвращаемое значение

// Функция возвращает стандартные коды HRESULT, включая следующие:

// S_FALSE	директория не существует.
// E_INVALIDARG	в nFolder указан неправильный CSIDL.

// Замечания

// Эта функция является разновидностью SHGetSpecialFolderPath, включённой в ранние версии оболочки и содержится в SHFolder.dll. Ниже приведено описание значений CSIDL:

// CSIDL_FLAG_CREATE
// Этот CSIDL можно совмещать с другими CSIDL, чтобы директория была создана в случае её отсутствия.
// CSIDL_ADMINTOOLS
// Директория "administrative tools" для конкретного пользователя.
// CSIDL_ALTSTARTUP
// Директория, отвечающая за "Автозагрузку" при нелокализованном запуске.
// CSIDL_APPDATA
// C:\Documents and Settings\username\Application Data.
// CSIDL_BITBUCKET
// Виртуальная директория содержащая объекты в пользовательской корзине (Recycle Bin).
// CSIDL_COMMON_ADMINTOOLS
// "administrative tools" для всех пользователей компьютера.
// CSIDL_COMMON_ALTSTARTUP
// Нелокализованная автозагрузка (Startup) для всех пользователей. Доступна только в семействе Windows NT.
// CSIDL_COMMON_APPDATA
// C:\Documents and Settings\All Users\Application Data.
// CSIDL_COMMON_DESKTOPDIRECTORY
// Рабочий стол для всех пользователей C:\Documents and Settings\All Users\Desktop. Доступна только в системах Windows NT.
// CSIDL_COMMON_DOCUMENTS
// Документы для всех пользователей C:\Documents and Settings\All Users\Documents. Доступна в системах Windows NT и Windows 95, 98 с установленной Shfolder.dll.
// CSIDL_COMMON_FAVORITES
// "Избранное" для всех пользователей. Доступна только в семействе Windows NT.
// CSIDL_COMMON_PROGRAMS
// Директория для меню "Пуск->Программы" для всех пользователей C:\Documents and Settings\All Users\Start Menu\Programs. Доступна только в семействе Windows NT.
// CSIDL_COMMON_STARTMENU
// Директория для меню "Пуск" C:\Documents and Settings\All Users\Start Menu. Доступна только в семействе Windows NT.
// CSIDL_COMMON_STARTUP
// C:\Documents and Settings\All Users\Start Menu\Programs\Startup. Доступна только в семействе Windows NT.
// CSIDL_COMMON_TEMPLATES
// C:\Documents and Settings\All Users\Templates. Доступна только в семействе Windows NT.
// CSIDL_CONTROLS
// Виртуальная директория, содержащая иконки для приложений Панели управления.
// CSIDL_COOKIES
// C:\Documents and Settings\username\Cookies.
// CSIDL_DESKTOP
// Виртуальная папка рабочего стола Windows.
// CSIDL_DESKTOPDIRECTORY
// Директория, которая физически хранит файловые объекты для рабочего стола (не путать с самой директорией рабочего стола). Обычно C:\Documents and Settings\username\Desktop.
// CSIDL_DRIVES
// Виртуальная директория "My Computer", которая содержит всё о локальном компьютере: устройства хранения, принтеры и Панель управления. Директория так же может содержать промапленные сетевые диски.
// CSIDL_FAVORITES
// "Избранное" для определённого пользователя C:\Documents and Settings\username\Favorites.
// CSIDL_FONTS
// Виртуальная директория, содержащая шрифты. Обычно C:\WINNT\Fonts.
// CSIDL_HISTORY
// Физическая директория, в которой хранится Internet history.
// CSIDL_INTERNET
// Виртуальная директория для интернета.
// CSIDL_INTERNET_CACHE
// C:\Documents and Settings\username\Temporary Internet Files.
// CSIDL_LOCAL_APPDATA
// C:\Documents and Settings\username\Local Settings\Application Data.
// CSIDL_MYMUSIC
// C:\Documents and Settings\User\My Documents\My Music.
// CSIDL_MYPICTURES
// C:\Documents and Settings\username\My Documents\My Pictures.
// CSIDL_NETHOOD
// C:\Documents and Settings\username\NetHood. Это не тоже, что CSIDL_NETWORK.
// CSIDL_NETWORK
// Сетевое окружение (Network Neighborhood) - виртуальная папка.
// CSIDL_PERSONAL
// C:\Documents and Settings\username\My Documents. Для доступа к этой виртуальной директории, используйте технологию, описанную в Управление файловой системой.
// CSIDL_PRINTERS
// Виртуальная директория с установленными принтерами.
// CSIDL_PRINTHOOD
// Директория, которая содержит ссылки на объекты, которые могут существовать в виртуальной папке Printers. Обычно C:\Documents and Settings\username\PrintHood.
// CSIDL_PROFILE
// Директория с профилем пользователя.
// CSIDL_PROGRAM_FILES
// C:\Program Files.
// CSIDL_PROGRAM_FILES_COMMON
// C:\Program Files\Common. Доступна только в системах Windows NT, Windows 2000, и Windows XP. Не доступна в Windows Millennium Edition (Windows Me).
// CSIDL_PROGRAMS
// C:\Documents and Settings\username\Start Menu\Programs.
// CSIDL_RECENT
// Директория, содержащая часто используемые документы C:\Documents and Settings\username\Recent. Чтобы создать ярлык в этой директории, можно воспользоваться функцией SHAddToRecentDocs.
// CSIDL_SENDTO
// C:\Documents and Settings\username\SendTo.
// CSIDL_STARTMENU
// C:\Documents and Settings\username\Start Menu.
// CSIDL_STARTUP
// C:\Documents and Settings\username\Start Menu\Programs\Startup.
// CSIDL_SYSTEM
// C:\WINNT\SYSTEM32.
// CSIDL_TEMPLATES
// Директория с шаблонами документов.
// CSIDL_WINDOWS
// Директория, прописанная в переменных окружения %windir% или %SYSTEMROOT%. Обычно C:\WINNT.
 