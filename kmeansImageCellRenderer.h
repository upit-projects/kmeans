#ifndef KMEANSIMAGECELLRENDERER_H
#define KMEANSMAGECELLRENDERER_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/grid.h>
#include <wx/image.h>

class kmeansImageCellRenderer : public wxGridCellStringRenderer
{
public:

   wxBitmap * cellBitmap;

   kmeansImageCellRenderer(wxImage image) {
        cellBitmap = new wxBitmap(image);
   }

   ~kmeansImageCellRenderer() {
        if (cellBitmap != nullptr) {
            delete cellBitmap;
        }
   }

   virtual void Draw(wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, const wxRect& rect, int row, int col, bool isSelected)
   {

      wxGridCellStringRenderer::Draw(grid, attr, dc, rect, row, col, isSelected);
      dc.DrawBitmap(*cellBitmap, rect.x, rect.y);

   }
};

#endif 
