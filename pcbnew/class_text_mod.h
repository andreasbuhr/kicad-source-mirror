/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2004 Jean-Pierre Charras, jaen-pierre.charras@gipsa-lab.inpg.com
 * Copyright (C) 1992-2016 KiCad Developers, see AUTHORS.txt for contributors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

/**
 * @file class_text_mod.h
 * @brief Footprint text class description.
 */


#ifndef TEXT_MODULE_H_
#define TEXT_MODULE_H_


#include <eda_text.h>
#include <class_board_item.h>


class LINE_READER;
class EDA_RECT;
class EDA_DRAW_PANEL;
class MODULE;
class MSG_PANEL_ITEM;


#define UMBILICAL_COLOR   LIGHTBLUE


class TEXTE_MODULE : public BOARD_ITEM, public EDA_TEXT
{
public:
    /** Text module type: there must be only one (and only one) for each
     * of the reference and value texts in one module; others could be
     * added for the user (DIVERS is French for 'others'). Reference and
     * value always live on silkscreen (on the module side); other texts
     * are planned to go on whatever layer the user wants (except
     * copper, probably) */
    enum TEXT_TYPE
    {
        TEXT_is_REFERENCE = 0,
        TEXT_is_VALUE = 1,
        TEXT_is_DIVERS = 2
    };

    TEXTE_MODULE( MODULE* parent, TEXT_TYPE text_type = TEXT_is_DIVERS );

    // Do not create a copy constructor & operator=.
    // The ones generated by the compiler are adequate.

    ~TEXTE_MODULE();

    static inline bool ClassOf( const EDA_ITEM* aItem )
    {
        return aItem && PCB_MODULE_TEXT_T == aItem->Type();
    }

    virtual const wxPoint& GetPosition() const override
    {
        return EDA_TEXT::GetTextPos();
    }

    virtual void SetPosition( const wxPoint& aPos ) override
    {
        EDA_TEXT::SetTextPos( aPos );
        SetLocalCoord();
    }

    void SetTextAngle( double aAngle );

    /// Rotate text, in footprint editor
    /// (for instance in footprint rotation transform)
    void Rotate( const wxPoint& aOffset, double aAngle ) override;

    /// Flip entity during module flip
    void Flip( const wxPoint& aCentre ) override;

    /// Mirror text position in footprint edition
    /// the text itself is not mirrored, and the layer not modified,
    /// only position is mirrored.
    /// (use Flip to change layer to its paired and mirror the text in fp editor).
    void Mirror( const wxPoint& aCentre, bool aMirrorAroundXAxis );

    /// move text in move transform, in footprint editor
    void Move( const wxPoint& aMoveVector ) override;

    /// @deprecated it seems (but the type is used to 'protect'
    //  reference and value from deletion, and for identification)
    void SetType( TEXT_TYPE aType )     { m_Type = aType; }
    TEXT_TYPE GetType() const           { return m_Type; }

    /**
     * Function SetEffects
     * sets the text effects from another instance.
     */
    void SetEffects( const TEXTE_MODULE& aSrc )
    {
        EDA_TEXT::SetEffects( aSrc );
        SetLocalCoord();
        // SetType( aSrc.GetType() );
    }

    /**
     * Function SwapEffects
     * swaps the text effects of the two involved instances.
     */
    void SwapEffects( TEXTE_MODULE& aTradingPartner )
    {
        EDA_TEXT::SwapEffects( aTradingPartner );
        SetLocalCoord();
        aTradingPartner.SetLocalCoord();
        // std::swap( m_Type, aTradingPartner.m_Type );
    }

    // The Pos0 accessors are for module-relative coordinates
    void SetPos0( const wxPoint& aPos ) { m_Pos0 = aPos; SetDrawCoord(); }
    const wxPoint& GetPos0() const      { return m_Pos0; }

    int GetLength() const;        // text length

    /**
     * @return the text rotation for drawings and plotting
     * the footprint rotation is taken in account
     */
    double GetDrawRotation() const;
    double GetDrawRotationRadians() const { return GetDrawRotation() * M_PI/1800; }

    // Virtual function
    const EDA_RECT GetBoundingBox() const override;

    ///> Set absolute coordinates.
    void SetDrawCoord();

    ///> Set relative coordinates.
    void SetLocalCoord();

    /* drawing functions */

    /**
     * Function Draw
     * Draw the text according to the footprint pos and orient
     * @param aPanel = draw panel, Used to know the clip box
     * @param aDC = Current Device Context
     * @param aOffset = draw offset (usually wxPoint(0,0)
     * @param aDrawMode = GR_OR, GR_XOR..
     */
    void Draw( EDA_DRAW_PANEL* aPanel,
               wxDC*           aDC,
               GR_DRAWMODE     aDrawMode,
               const wxPoint&  aOffset = ZeroOffset ) override;

    /**
     * Function DrawUmbilical
     * draws a line from the TEXTE_MODULE origin
     * to parent MODULE origin.
     * @param aPanel = the current DrawPanel
     * @param aDC = the current device context
     * @param aDrawMode = drawing mode, typically GR_XOR
     * @param aOffset = offset for TEXTE_MODULE
     */
    void DrawUmbilical( EDA_DRAW_PANEL* aPanel,
                        wxDC*           aDC,
                        GR_DRAWMODE     aDrawMode,
                        const wxPoint&  aOffset = ZeroOffset );

    void GetMsgPanelInfo( std::vector< MSG_PANEL_ITEM >& aList ) override;

    virtual bool HitTest( const wxPoint& aPosition ) const override
    {
        return TextHitTest( aPosition );
    }

    virtual bool HitTest( const EDA_RECT& aRect, bool aContained = false, int aAccuracy = 0 ) const override
    {
        return TextHitTest( aRect, aContained, aAccuracy );
    }

    wxString GetClass() const override
    {
        return wxT( "MTEXT" );
    }

    wxString GetSelectMenuText() const override;

    BITMAP_DEF GetMenuImage() const override;

    EDA_ITEM* Clone() const override;

    virtual wxString GetShownText() const override;

    /// @copydoc VIEW_ITEM::ViewBBox()
    virtual const BOX2I ViewBBox() const override;

    /// @copydoc VIEW_ITEM::ViewGetLayers()
    virtual void ViewGetLayers( int aLayers[], int& aCount ) const override;

    /// @copydoc VIEW_ITEM::ViewGetLOD()
    virtual unsigned int ViewGetLOD( int aLayer, KIGFX::VIEW* aView ) const override;

#if defined(DEBUG)
    virtual void Show( int nestLevel, std::ostream& os ) const override { ShowDummy( os ); }
#endif

private:
    /* Note: orientation in 1/10 deg relative to the footprint
     * Physical orient is m_Orient + m_Parent->m_Orient
     */

    TEXT_TYPE m_Type;       ///< 0=ref, 1=val, etc.

    wxPoint   m_Pos0;       ///< text coordinates relative to the footprint anchor, orient 0.
                            ///< text coordinate ref point is the text center
};

#endif // TEXT_MODULE_H_
