import bb.cascades 1.2

Page {    
    property alias fileListPageTitle: titlebar.title
    property alias fileListPagePath: title_path.text
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
            
            dataModel: _app.model
                        
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
				var chosenItem = dataModel.data(indexPath);
                var path = chosenItem.path
                if(_app.readDir(path)){
                    var nextPage = page.createObject();
                    nextPage.fileListPagePath = _app.getPath(path)
                    nextPage.fileListPageTitle = _app.getTitle(path)
                    navPane.push(nextPage);                    
                }
            }
        }
    }    
    
    attachedObjects: [
        ComponentDefinition {
            id: page
            source: "FileListPage.qml"
        }
    ]
}
