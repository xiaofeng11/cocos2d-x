﻿/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#ifndef __CC_FAST_TMX_LAYER_H__
#define __CC_FAST_TMX_LAYER_H__

#include <map>
#include <unordered_map>
#include "2d/CCNode.h"
#include "2d/CCTMXXMLParser.h"
#include "renderer/CCPrimitiveCommand.h"
#include "base/CCMap.h"

NS_CC_BEGIN

class TMXMapInfo;
class TMXLayerInfo;
class TMXTilesetInfo;
class Texture2D;
class Sprite;
struct _ccCArray;

namespace experimental{

/**
 * @addtogroup _2d
 * @{
 */

/** @brief @~english FastTMXLayer represents the TMX layer.

 * It is a subclass of SpriteBatchNode. By default the tiles are rendered using a TextureAtlas.
 * If you modify a tile on runtime, then, that tile will become a Sprite, otherwise no Sprite objects are created.
 * The benefits of using Sprite objects as tiles are:
 * - tiles (Sprite) can be rotated/scaled/moved with a nice API.

 * If the layer contains a property named "cc_vertexz" with an integer (in can be positive or negative),
 * then all the tiles belonging to the layer will use that value as their OpenGL vertex Z for depth.

 * On the other hand, if the "cc_vertexz" property has the "automatic" value, then the tiles will use an automatic vertex Z value.
 * Also before drawing the tiles, GL_ALPHA_TEST will be enabled, and disabled after drawing them. The used alpha func will be:

 * glAlphaFunc( GL_GREATER, value ).

 * "value" by default is 0, but you can change it from Tiled by adding the "cc_alpha_func" property to the layer.
 * The value 0 should work for most cases, but if you have tiles that are semi-transparent, then you might want to use a different
 * value, like 0.5.
 
 * For further information, please see the programming guide:
 * http://www.cocos2d-iphone.org/wiki/doku.php/prog_guide:tiled_maps
 
 * @~chinese FastTMXLayer用来表示TMX layer，
 * 继承自SpriteBatchNode。tiles默认使用TextureAtlas进行渲染。
 * 如果运行时修改一个tile，那么tile将变成一个Sprite，反之，则不会有Sprite对象被创建。
 * 使用Sprite对象作为Tile的好处是:
 * -  tiles(即Sprite)可以通过完善的API进行旋转/缩放/移动
 * 如果layer包含一个属性名为"cc_vertexz"的整数(正整数/负整数)，那么属于layer的tiles将使用该属性值作为它们OpenGL用来渲染显示层次的Z值。
 * 另外,如果“cc_vertexz”属性值为“automatic”，那么这些tiles将使用一个自分配的Z值。
 * 在绘制这些tiles时，在绘制前，必须设置GL_ALPHA_TEST为可用，绘制后设置为禁用。使用的Alpha函数如下：
 * glAlphaFunc( GL_GREATER, value ).
 * “value”默认为0,但是你可以通过向层添加“cc_alpha_func”属性来改变该值.
 * 大多数情况value的值是0，但如果有些tiles是半透明的，那么该值则可能会有所不同，比如0.5。
 * 进一步的信息,请参见编程指南(注：此链接已不可用):
 * http://www.cocos2d-iphone.org/wiki/doku.php/prog_guide:tiled_maps
 * 
 * @since v3.2
 * @js NA
 */

class CC_DLL TMXLayer : public Node
{
public:
    /** @~english Creates a FastTMXLayer with an tileset info, a layer info and a map info.
     *
     * @~chinese 通过指定TMXTilesetInfo,TMXLayerInfo和TMXMapInfo创建一个TMXLayer
     * 
     * @param tilesetInfo @~english An tileset info.
     * @~chinese TMXTilesetInfo数据.
     * @param layerInfo @~english A layer info.
     * @~chinese TMXLayerInfo数据.
     * @param mapInfo @~english A map info.
     * @~chinese TMXMapInfo数据.
     * @return @~english An autorelease object.
     * @~chinese 一个autorelease对象.
     */
    static TMXLayer * create(TMXTilesetInfo *tilesetInfo, TMXLayerInfo *layerInfo, TMXMapInfo *mapInfo);
    /**
     * @js ctor
     */
    TMXLayer();
    /**
     * @js NA
     * @lua NA
     */
    virtual ~TMXLayer();

    /** @~english Returns the tile gid at a given tile coordinate. It also returns the tile flags.
     * 
     * @~chinese 通过给定的瓦片坐标、flags（可选）返回瓦片GID.
     * 该方法要求瓦片地图之前没有被释放过(如：没有调用过layer->releaseMap()).
     * 
     * @param tileCoordinate @~english The tile coordinate.
     * @~chinese 瓦片坐标.
     * @param flags @~english Tile flags.
     * @~chinese 瓦片flags.
     * @return @~english Returns the tile gid at a given tile coordinate. It also returns the tile flags.
     * @~chinese 返回过给定的瓦片坐标、flags（可选）的瓦片GID.
     */
    int getTileGIDAt(const Vec2& tileCoordinate, TMXTileFlags* flags = nullptr);

    /** @~english Sets the tile gid (gid = tile global id) at a given tile coordinate.
     * The Tile GID can be obtained by using the method "tileGIDAt" or by using the TMX editor -> Tileset Mgr +1.
     * If a tile is already placed at that position, then it will be removed.
     * @~chinese 设置给定坐标的瓦片的gid(gid =瓦片全局id)
     * 瓦片GID可以使用方法“tileGIDAt”来获得，或使用TMX编辑器 -> Tileset Mgr +1得到.
     * 如果该位置上已有一个tile，那么该位置上已有的tile将会被移除。
     * 
     * @param gid @~english The tile gid.
     * @~chinese 瓦片gid.
     * @param tileCoordinate @~english The tile coordinate.
     * @~chinese 瓦片坐标.
     */
    void setTileGID(int gid, const Vec2& tileCoordinate);

    /** @~english Sets the tile gid (gid = tile global id) at a given tile coordinate.
     * The Tile GID can be obtained by using the method "tileGIDAt" or by using the TMX editor -> Tileset Mgr +1.
     * If a tile is already placed at that position, then it will be removed.
     * Use withFlags if the tile flags need to be changed as well.
     *
     * @~chinese 设置给定坐标、flags的瓦片的gid(gid =瓦片全局id)
     * 瓦片GID可以使用方法“tileGIDAt”来获得，或使用TMX编辑器 -> Tileset Mgr +1.
     * 如果一个瓦片已经放在那个位置,那么它将被删除.
     * 使用flags如果瓦片flags需要更改.
     * 
     * @param gid @~english The tile gid.
     * @~chinese 瓦片gid.
     * @param tileCoordinate @~english The tile coordinate.
     * @~chinese 瓦片坐标.
     * @param flags @~english The tile flags.
     * @~chinese 瓦片的flags.
     */
    void setTileGID(int gid, const Vec2& tileCoordinate, TMXTileFlags flags);

    /** @~english Removes a tile at given tile coordinate.
     *
     * @~chinese 删除指定坐标上的瓦片.
     * 
     * @param tileCoordinate @~english The tile coordinate.
     * @~chinese 瓦片坐标.
     */
    void removeTileAt(const Vec2& tileCoordinate);

    /** @~english Returns the position in points of a given tile coordinate.
     *
     * @~chinese 获取指定坐标的位置(以点为单位)
     * 
     * @param tileCoordinate @~english The tile coordinate.
     * @~chinese 瓦片坐标.
     * @return @~english The position in points of a given tile coordinate.
     * @~chinese 给定瓦片坐标点的位置.
     */
    Vec2 getPositionAt(const Vec2& tileCoordinate);

    /** @~english Return the value for the specific property name.
     *
    * @~chinese 获取指定属性名(propertyName)的值
     * 
     * @param propertyName @~english The specific property name.
     * @~chinese 特定的属性名.
     * @return @~english Return the value for the specific property name.
     * @~chinese 返回指定属性名(propertyName)的值.
     */
    Value getProperty(const std::string& propertyName) const;

    /** @~english Creates the tiles.  @~chinese 创造瓦片。*/
    void setupTiles();
    
    /** @~english Get the tile layer name.
     *
     * @~chinese 获取层的名称.
     * 
     * @return @~english The layer name.
     * @~chinese 层的名称.
     */
    inline const std::string& getLayerName(){ return _layerName; }
    
    /** @~english Set the tile layer name.
     *
     * @~chinese 设置层的名称.
     * 
     * @param layerName @~english The layer name.
     * @~chinese 层的名称.
     */
    inline void setLayerName(const std::string& layerName){ _layerName = layerName; }

    /** @~english Size of the layer in tiles.
     *
     * @~chinese 获取层的尺寸.
     * 
     * @return @~english Size of the layer in tiles.
     * @~chinese 瓦片层的尺寸.
     */
    inline const Size& getLayerSize() const { return _layerSize; };
    
    /** @~english Set the size of the layer in tiles. 
     *
     * @~chinese 设置层瓦片层的尺寸.
     * 
     * @param size @~english Size of the layer in tiles.
     * @~chinese 瓦片层的尺寸.
     */
    inline void setLayerSize(const Size& size) { _layerSize = size; };
    
    /** @~english Size of the map's tile (could be different from the tile's size).
     *
     * @~chinese 获取瓦片的尺寸(瓦片的尺寸可能会有所不同).
     * 
     * @return @~english The size of the map's tile.
     * @~chinese 瓦片的尺寸(瓦片的尺寸可能会有所不同).
     */
    inline const Size& getMapTileSize() const { return _mapTileSize; };
    
    /** @~english Set the size of the map's tile. 
     *
     * @~chinese 设置瓦片的尺寸.
     * 
     * @param size @~english The size of the map's tile.
     * @~chinese 瓦片的尺寸.
     */
    inline void setMapTileSize(const Size& size) { _mapTileSize = size; };
    
    /** @~english Pointer to the map of tiles.
     * @~chinese 获取指向tiles地图的指针.
     *
     * @return @~english Pointer to the map of tiles.
     * @~chinese 指向tiles地图的指针.
     *
     * @js NA
     * @lua NA
     */
    const uint32_t* getTiles() const { return _tiles; };
    
    /** @~english Set the pointer to the map of tiles.
     *
     * @~chinese 设置指向tiles地图的指针。
     * 
     * @param tiles @~english A pointer to the map of tiles.
     * @~chinese 指向tiles地图的指针
     */
    void setTiles(uint32_t* tiles) { _tiles = tiles; _quadsDirty = true;};
    
    /** @~english Tileset information for the layer.
     *
     * @~chinese 获取层的tileset信息。
     * 
     * @return @~english Tileset information for the layer.
     * @~chinese 层的tileset信息。
     */
    inline TMXTilesetInfo* getTileSet() const { return _tileSet; };
    
    /** @~english Set the tileset information for the layer. 
     *
     * @~chinese 设置layer的Tileset信息
     * 
     * @param info @~english The tileset information for the layer.
     * @~chinese layer的Tileset信息
     */
    inline void setTileSet(TMXTilesetInfo* info) {
        CC_SAFE_RETAIN(info);
        CC_SAFE_RELEASE(_tileSet);
        _tileSet = info;
    };
    
    /** @~english Layer orientation, which is the same as the map orientation.
     *
     * @~chinese  获取Layer方向(同地图方向)
     * 
     * @return @~english Layer orientation, which is the same as the map orientation.
     * @~chinese  Layer方向(同地图方向)
     */
    inline int getLayerOrientation() const { return _layerOrientation; };
    
    /** @~english Set Layer orientation, which is the same as the map orientation. 
     *
     * @~chinese 设置 Layer方向(同地图方向)
     * 
     * @param orientation @~english Layer orientation,which is the same as the map orientation.
     * @~chinese  Layer方向(同地图方向)
     */
    inline void setLayerOrientation(int orientation) { _layerOrientation = orientation; };
    
    /** @~english Properties from the layer. They can be added using Tiled. 
     *
     * @~chinese 获取layer的属性，可以被当作Tile添加
     * 
     * @return @~english Properties from the layer. They can be added using Tiled.
     * @~chinese layer的属性
     */
    inline const ValueMap& getProperties() const { return _properties; };
    
    /** @~english Properties from the layer. They can be added using Tiled.
     *
     * @~chinese 获取layer的属性，可以被当作Tile添加
     * 
     * @return @~english Properties from the layer. They can be added using Tiled.
     * @~chinese layer的属性
     */
    inline ValueMap& getProperties() { return _properties; };
    
    /** @~english Set the properties to the layer.
     *
     * @~chinese 设置层属性.
     * 
     * @param properties @~english It is used to set the layer Properties.
     * @~chinese 它是用来设置图层的属性.
     */
    inline void setProperties(const ValueMap& properties)
    {
        _properties = properties;
    };

    /** @~english Returns the tile (Sprite) at a given a tile coordinate.
     * The returned Sprite will be already added to the TMXLayer. Don't add it again.
     * The Sprite can be treated like any other Sprite: rotated, scaled, translated, opacity, color, etc.
     * You can remove either by calling:
     * - layer->removeChild(sprite, cleanup);
     *
     * @~chinese  通过指定的tile坐标获取对应的tile(Sprite)。
     * 返回的tile(Sprite)应是已经添加到TMXLayer，请不要重复添加。
     * 这个tile(Sprite)如同其他的Sprite一样，可以旋转、缩放、翻转、透明化、设置颜色等。
     * 你可以通过调用以下方法来对它进行删除:
     * - layer->removeChild(sprite, cleanup);
     * 
     * @param tileCoordinate @~english A tile coordinate.
     * @~chinese 瓦片坐标.
     * @return @~english Returns the tile (Sprite) at a given a tile coordinate.
     * @~chinese 返回指定坐标位置的瓦片(Sprite).
     */
    Sprite* getTileAt(const Vec2& tileCoordinate);
    
    /** @~english Set an sprite to the tile,with the tile coordinate and gid.
     *
     * @~chinese 设置一个瓦片的sprite。
     * 
     * @param sprite @~english A Sprite.
     * @~chinese 一个Sprite。
     * @param pos @~english The tile coordinate.
     * @~chinese 瓦片坐标。
     * @param gid @~english The tile gid.
     * @~chinese 瓦片gid。
     */
    void setupTileSprite(Sprite* sprite, Vec2 pos, int gid);

    //
    // Override
    //
    virtual std::string getDescription() const override;
    virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags) override;
    void removeChild(Node* child, bool cleanup = true) override;

protected:

    bool initWithTilesetInfo(TMXTilesetInfo *tilesetInfo, TMXLayerInfo *layerInfo, TMXMapInfo *mapInfo);
    void updateTiles(const Rect& culledRect);
    Vec2 calculateLayerOffset(const Vec2& offset);

    /* @~english The layer recognizes some special properties, like cc_vertez  @~chinese 层认识到一些特殊的属性,比如cc_vertez*/
    void parseInternalProperties();
    
    Mat4 tileToNodeTransform();
    Rect tileBoundsForClipTransform(const Mat4 &tileToClip);
    
    int getVertexZForPos(const Vec2& pos);
    
    //Flip flags is packed into gid
    void setFlaggedTileGIDByIndex(int index, int gid);
    
    //
    void updateTotalQuads();
    
    void onDraw(Primitive* primitive);
    inline int getTileIndexByPos(int x, int y) const { return x + y * (int) _layerSize.width; }
    
    void updateVertexBuffer();
    void updateIndexBuffer();
    void updatePrimitives();
protected:
    
    //! name of the layer
    std::string _layerName;

    /** @~english size of the layer in tiles  @~chinese 以瓦片为单位的层的大小*/
    Size _layerSize;
    /** @~english size of the map's tile (could be different from the tile's size)  @~chinese 地图瓦片的大小(瓦片的大小可能会有所不同)*/
    Size _mapTileSize;
    /** @~english pointer to the map of tiles  @~chinese 指向地图瓦片的指针*/
    uint32_t* _tiles;
    /** @~english Tileset information for the layer  @~chinese 确定信息层*/
    TMXTilesetInfo* _tileSet;
    /** @~english Layer orientation, which is the same as the map orientation  @~chinese 层取向,它是一样的地图定位*/
    int _layerOrientation;
    /** @~english properties from the layer. They can be added using Tiled  @~chinese The properties from the layer. They can be added using Tiled*/
    ValueMap _properties;

    Texture2D *_texture;
    
    /** @~english container for sprite children. map<index, pair<sprite, gid> >  @~chinese Container for Sprite children. The map < index, pair < Sprite, gid > >*/
    std::map<int, std::pair<Sprite*, int> > _spriteContainer;

    //GLuint _buffersVBO; //0: vertex, 1: indices

    Size _screenGridSize;
    Rect _screenGridRect;
    int _screenTileCount;
    
    int _vertexZvalue;
    bool _useAutomaticVertexZ;
    
    /** @~english tile coordinate to node coordinate transform  @~chinese 瓷砖协调节点坐标变换*/
    Mat4 _tileToNodeTransform;
    /** @~english data for rendering  @~chinese 数据呈现*/
    bool _quadsDirty;
    std::vector<int> _tileToQuadIndex;
    std::vector<V3F_C4B_T2F_Quad> _totalQuads;
    std::vector<GLushort> _indices;
    std::map<int/*vertexZ*/, int/*offset to _indices by quads*/> _indicesVertexZOffsets;
    std::unordered_map<int/*vertexZ*/, int/*number to quads*/> _indicesVertexZNumber;
    std::vector<PrimitiveCommand> _renderCommands;
    bool _dirty;
    
    VertexBuffer* _vertexBuffer;
    
    VertexData* _vData;
    
    IndexBuffer* _indexBuffer;
    
    Map<int , Primitive*> _primitives;
    
public:
    /** @~english Possible orientations of the TMX map  @~chinese TMX的可能方向地图*/
    static const int FAST_TMX_ORIENTATION_ORTHO;
    static const int FAST_TMX_ORIENTATION_HEX;
    static const int FAST_TMX_ORIENTATION_ISO;
};

// end of tilemap_parallax_nodes group
/// @}
} //end of namespace experimental
NS_CC_END

#endif //__CCTMX_LAYER2_H__
