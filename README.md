# What is Hend

C and C++ is known to be one of the most used system programming languages out there, these languages can be used to accomplish countless task, Hend makes use of C++. In this *article*, I will explain the major software design behind Hend. Hend is basically a PC utility software that helps in searching and downloading youtube videos, it is written in C++ using the popular cross-platform framework Qt.  Hend currently have very few features which includes:

* Performing basic and advanced search for videos using the Youtube API
* Showing detailed description of the search results
* Performing filters based on relevance, dates of videos
* It supports concurrent downloads
* Pause and resume downloads
* All available video and audio formats, resolution for videos and their sizes.

and I hope to **add** more -- and **improve** the existing -- features in the coming future( most especially showing video thumbnails and preview ). The purpose of this article is to explain some of the features of C++11 and Qt I used when developing Hend. Let's dive right into the major design. Hend currently has 5 major components that makes up the whole software, the components include but not limited to

* the mainwindow,
* the search manager,
* the response parser/separator,
* video format specifier,
* and the download manager.

The following questions and others will be answered in this article and the next articles that follow this
* What does the MainWindow class handle specifically?
* How do you search for Youtube videos?
* How can you parse the response from Youtube?
* How do you download Youtube Videos?
* How do you filter results?


## MainWindow

The Mainwindow( located in /src/mainwindow.hpp and /src/mainwindow.cpp ) is the entry point for Hend, it is called from the main function. The creation of the MainWindow does a little bit of initialization of components here and there, the menu bar, toolbar, the actions in the toolbar and the menu bar, the table displaying the video results, the available filters on the right hand side of the mainwindow. It then proceed to setup the menubar, toolbar and connects the action events/signals to their proper callbacks/slots. I'll explain briefly some of the operations performed in the [connectAllActions]( http://github.com/iamOgunyinka/Hend/) member function.

```
    void MainWindow::connectActions()
    {
        ...
        QObject::connect( m_directDownloadAction, SIGNAL( triggered() ), this, SLOT( findDownloadLink() ) );
        QObject::connect( m_basicSearchAction, SIGNAL( triggered() ), this, SLOT( basicSearch() ) );
        QObject::connect( m_advancedSearchAction, SIGNAL( triggered() ), this, SLOT( advancedSearch() ) );
        ...
    }
```
