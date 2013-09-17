import bb.cascades 1.2

Page {
    property alias imageViewPageTitle: title_bar.title
    property alias imageViewPageImage: image_view.image

    titleBar: TitleBar {
        id: title_bar
    }

    ImageView {
        id: image_view
    }
}
