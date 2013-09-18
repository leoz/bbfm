import bb.cascades 1.2
import com.leoz 1.0

Page {    
    property alias fileListPageTitle: titlebar.title
    property alias fileListPagePath: title_path.text
    property alias fileListPageModel: list_model
    
    titleBar: TitleBar {
        id: titlebar
    }

    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        
        layout: StackLayout {
            orientation: LayoutOrientation.TopToBottom
        }
        
        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Top
            
            background: Color.DarkGray
            
            bottomPadding: 2

            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Top

                background: Color.White
                
                leftPadding: 10
                
                
                Label {
                    id: title_path
                    
                    textStyle {
                        base: SystemDefaults.TextStyles.SubtitleText
                        color: Color.DarkGray
                    }
                }                
            }
        }

        ListView {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Bottom
            
            dataModel: list_model
                        
            listItemComponents: [
                ListItemComponent {
                    type: ""
                    StandardListItem {
                        image: ListItemData.icon
                        title: ListItemData.name
                        description: ListItemData.date
                        status: ListItemData.size
                    }
                }
            ]

            onTriggered: {
				var chosenItem = dataModel.data(indexPath)
                var path = chosenItem.path
                if(_app.showFileList(path)){
                    var nextPage = file_list_page.createObject()
                    nextPage.fileListPagePath = _app.getPath(path)
                    nextPage.fileListPageTitle = _app.getTitle(path)
                    navPane.push(nextPage);                    
                }
                else {
                    if(_app.showImageView(path)) {
                        var p = image_view_page.createObject()
                        p.imageViewPageData.reset()
                        p.imageViewPageData.path = _app.getPath(path)
                        p.imageViewPageData.setSize(2)
                        p.imageViewPageData.load()
                        p.imageViewPageTitle = _app.getTitle(path)
                        navPane.push(p);                                            
                    }
                }
            }
            
            attachedObjects: [
                FileDataListModel {
                    id: list_model
                }                
            ]
        }
    }
    
    onFileListPagePathChanged: {
        list_model.setDir(fileListPagePath)
    }   
    
    attachedObjects: [
        ComponentDefinition {
            id: file_list_page
            source: "FileListPage.qml"
        },
        ComponentDefinition {
            id: image_view_page
            source: "ImageViewPage.qml"            
        }
    ]
}
