import bb.cascades 1.2

Page {    
    property alias fileListPageTitle: title_name.text
    property alias fileListPagePath: title_path.text
    titleBar: TitleBar {
        id: titlebar
        kind: TitleBarKind.FreeForm
        kindProperties: FreeFormTitleBarKindProperties {
            Container {
                layout: StackLayout { orientation: LayoutOrientation.TopToBottom }
                leftPadding: 10
                rightPadding: 10
                Label {
                    id: title_name
                    textStyle {
                        color: Color.White
                    }
                    verticalAlignment: VerticalAlignment.Center
                    layoutProperties: StackLayoutProperties { spaceQuota: 1 }
                }
                Label {
                    id: title_path
                    verticalAlignment: VerticalAlignment.Center
                }
            }
        }
    }

    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        layout: DockLayout {
        }
        ListView {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Top
            
            dataModel: _app.model
                        
            listItemComponents: [
                ListItemComponent {
                    type: ""
                    StandardListItem {
                        image: ListItemData.image
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
