/***************************************************************************
    qgsapplayerhandling.h
    -------------------------
    begin                : July 2022
    copyright            : (C) 2022 by Nyall Dawson
    email                : nyall dot dawson at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QGSAPPLAYERHANDLING_H
#define QGSAPPLAYERHANDLING_H

#include "qgis.h"
#include "qgis_app.h"
#include "qgsconfig.h"
#include "qgsmaplayer.h"
#include "qgsvectorlayerref.h"

class QgsMapLayer;
class QgsProviderSublayerDetails;
class QgsPointCloudLayer;
class QgsVectorLayer;
class QgsRasterLayer;
class QgsMeshLayer;
class QgsPluginLayer;
class QgsVectorTileLayer;

/**
 * Contains logic related to general layer handling in QGIS app.
 */
class APP_EXPORT QgsAppLayerHandling
{

  public:

    enum class SublayerHandling
    {
      AskUser,
      LoadAll,
      AbortLoading
    };

    /**
     * Add a vector layer directly without prompting user for location
     * The caller must provide information compatible with the provider plugin
     * using the \a vectorLayerPath and \a baseName. The provider can use these
     * parameters in any way necessary to initialize the layer. The \a baseName
     * parameter is used in the Map Legend so it should be formed in a meaningful
     * way.
     */
    static QgsVectorLayer *addVectorLayer( const QString &vectorLayerPath, const QString &baseName, const QString &providerKey = QLatin1String( "ogr" ) );

    /**
     * Add a raster layer directly without prompting user for location
     * The caller must provide information compatible with the provider plugin
     * using the \a uri and \a baseName. The provider can use these
     * parameters in any way necessary to initialize the layer. The \a baseName
     * parameter is used in the Map Legend so it should be formed in a meaningful
     * way.
     */
    static QgsRasterLayer *addRasterLayer( QString const &uri, QString const &baseName, QString const &providerKey = QLatin1String( "gdal" ) );

    /**
     * Adds a mesh layer directly without prompting user for location
     * The caller must provide information compatible with the provider plugin
     * using the \a url and \a baseName. The provider can use these
     * parameters in any way necessary to initialize the layer. The \a baseName
     * parameter is used in the Map Legend so it should be formed in a meaningful
     * way.
     */
    static QgsMeshLayer *addMeshLayer( const QString &url, const QString &baseName, const QString &providerKey );

    //! Open a point cloud layer - this is the generic function which takes all parameters
    static QgsPointCloudLayer *addPointCloudLayer( const QString &uri,
        const QString &baseName,
        const QString &providerKey,
        bool guiWarning = true );

    //! Open a plugin layer using its provider
    static QgsPluginLayer *addPluginLayer( const QString &uri, const QString &baseName, const QString &providerKey );

    //! Open a vector tile layer - this is the generic function which takes all parameters
    static QgsVectorTileLayer *addVectorTileLayer( const QString &uri, const QString &baseName, bool guiWarning = true );

    static void postProcessAddedLayer( QgsMapLayer *layer );

    static bool addVectorLayers( const QStringList &layers, const QString &enc, const QString &dataSourceType, bool guiWarning = true );


    /**
     * This method will open a dialog so the user can select GDAL sublayers to load
     * \returns TRUE if any items were loaded
     */
    static bool askUserForZipItemLayers( const QString &path, const QList< QgsMapLayerType > &acceptableTypes );


    static SublayerHandling shouldAskUserForSublayers( const QList< QgsProviderSublayerDetails > &layers, bool hasNonLayerItems = false );

    static QList< QgsMapLayer * > addSublayers( const QList< QgsProviderSublayerDetails> &layers, const QString &baseName, const QString &groupName );


    /**
     * Open a raster or vector file; ignore other files.
     * Used to process a commandline argument, FileOpen or Drop event.
     * Set \a allowInteractive to TRUE if it is OK to ask the user for information (mostly for
     * when a vector layer has sublayers and we want to ask which sublayers to use).
     * \returns TRUE if the file is successfully opened
     */
    static bool openLayer( const QString &fileName, bool allowInteractive = false );

    template<typename T> static T *addLayerPrivate( QgsMapLayerType type, const QString &uri, const QString &baseName, const QString &providerKey, bool guiWarnings = true );


    /**
     * Overloaded version of the private addRasterLayer()
     * Method that takes a list of file names instead of prompting
     * user with a dialog.
     * \returns TRUE if successfully added layer(s)
     */
    static bool addRasterLayers( const QStringList &files, bool guiWarning = true );

    //! Add a 'pre-made' map layer to the project
    static void addMapLayer( QgsMapLayer *mapLayer );

    static void openLayerDefinition( const QString &filename );

    //! Add a Layer Definition file
    static void addLayerDefinition();

    //! Add a list of database layers to the map
    static void addDatabaseLayers( const QStringList &layerPathList, const QString &providerKey );

    /**
     * Searches for layer dependencies by querying the form widgets and the
     * \a vectorLayer itself for broken relations. Style \a categories can be
     * used to limit the search to one or more of the currently implemented search
     * categories ("Forms" for the form widgets and "Relations" for layer weak relations).
     * \return a list of weak references to broken layer dependencies
     */
    static const QList< QgsVectorLayerRef > findBrokenLayerDependencies( QgsVectorLayer *vectorLayer,
        QgsMapLayer::StyleCategories categories = QgsMapLayer::StyleCategory::AllStyleCategories );

    /**
     * Scans the \a vectorLayer for broken dependencies and automatically
     * try to load the missing layers, users are notified about the operation
     * result. Style \a categories can be
     * used to exclude one of the currently implemented search categories
     * ("Forms" for the form widgets and "Relations" for layer weak relations).
     */
    static void resolveVectorLayerDependencies( QgsVectorLayer *vectorLayer,
        QgsMapLayer::StyleCategories categories = QgsMapLayer::AllStyleCategories );


    /**
     * Scans the \a vectorLayer for weak relations and automatically
     * try to resolve and create the broken relations.
     */
    static void resolveVectorLayerWeakRelations( QgsVectorLayer *vectorLayer );

    /**
     * Triggered when a vector layer style has changed, checks for widget config layer dependencies
     * \param categories style categories
     */
    static void onVectorLayerStyleLoaded( QgsVectorLayer *vl, const QgsMapLayer::StyleCategories categories );



};

#endif // QGSAPPLAYERHANDLING_H
