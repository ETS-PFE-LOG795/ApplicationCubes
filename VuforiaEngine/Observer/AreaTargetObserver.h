/*===============================================================================
Copyright (c) 2021 PTC Inc. All Rights Reserved.

Vuforia is a trademark of PTC Inc., registered in the United States and other
countries.
===============================================================================*/

#ifndef _VU_AREATARGETOBSERVER_H_
#define _VU_AREATARGETOBSERVER_H_

/**
 * \file AreaTargetObserver.h
 * \brief Observer for the AreaTarget feature
 */

#include <VuforiaEngine/Engine/Engine.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** \addtogroup AreaTargetObserverGroup Area Target Feature
 * \{
 */

 /// \brief Configuration error for Area Target creation
VU_ENUM(VuAreaTargetCreationError)
{
    VU_AREA_TARGET_CREATION_ERROR_NONE                         = 0x0, ///< No error
    VU_AREA_TARGET_CREATION_ERROR_INTERNAL                     = 0x1, ///< An internal error occurred while creating the observer
    VU_AREA_TARGET_CREATION_ERROR_AUTOACTIVATION_FAILED        = 0x2, ///< Observer auto-activation failed (probably due to the total target size limit)
    VU_AREA_TARGET_CREATION_ERROR_FEATURE_NOT_SUPPORTED        = 0x3, ///< Feature not supported (e.g. if Vuforia is not running on a 
                                                                      ///< platform-based Fusion Provider such as ARKit, ARCore)
    VU_AREA_TARGET_CREATION_ERROR_INVALID_DEVICE_POSE_OBSERVER = 0x4, ///< Device pose observer is null or invalid
    VU_AREA_TARGET_CREATION_ERROR_DATABASE_LOAD_ERROR          = 0x5, ///< Database file not found or an error occurred when reading data from it
                                                                      ///< (potentially unknown or corrupted file)
    VU_AREA_TARGET_CREATION_ERROR_INVALID_TARGET_NAME          = 0x6, ///< Invalid target name
    VU_AREA_TARGET_CREATION_ERROR_TARGET_NOT_FOUND             = 0x7  ///< Specified target not found in database
};

/// \brief Configuration for creating an Area Target observer
typedef struct VuAreaTargetConfig
{
    /// \brief Pointer to a device pose observer
    /// The Area Target observer will only report observations to the state while there
    /// is an active device pose observer
    VuObserver* devicePoseObserver;
    
    /// \brief Path to database containing targets
    const char* databasePath;
    
    /// \brief Target name
    const char* targetName;

    /// \brief Observer activation
    /**
     * \note The default value is VU_TRUE.
     */
    VuBool activate;
    
    /// \brief Offset from the origin of the target to the pose reported by an observation, relative to the target's frame of reference
    /**
     * \note The default value is identity matrix.
     */
    VuMatrix44F poseOffset;
} VuAreaTargetConfig;

/// \brief Default Area Target configuration
/**
 * \note Use this function to initialize the VuAreaTargetConfig data structure with default values.
 */
VU_API VuAreaTargetConfig VU_API_CALL vuAreaTargetConfigDefault();

/// \brief Target info for Area Target from its respective observation
typedef struct VuAreaTargetObservationTargetInfo
{
    /// \brief Persistent system-wide unique ID associated with the Area Target
    /**
     * \note The unique ID can't be changed.
     */
    const char* uniqueId;
    
    /// \brief Target name
    const char* name;
    
    /// \brief Size (dimensions) of the Area Target in meters
    VuVector3F size;

    /// \brief Axis-aligned bounding box of the observed target, relative to the target's frame of reference
    VuAABB bbox;

    /// \brief Whether the target requires external positions for initial detection
    VuBool requiresExternalPositions;

    /// \brief Pose offset used with the Area Target
    VuMatrix44F poseOffset;
} VuAreaTargetObservationTargetInfo;

/// \brief status info for an Area Target observation
VU_ENUM(VuAreaTargetObservationStatusInfo)
{
    VU_AREA_TARGET_OBSERVATION_STATUS_INFO_NORMAL       = 0x1, ///< Tracking working normally
    VU_AREA_TARGET_OBSERVATION_STATUS_INFO_NOT_OBSERVED = 0x2, ///< Target is not detected
    VU_AREA_TARGET_OBSERVATION_STATUS_INFO_RELOCALIZING = 0x3  ///< The tracking system is currently relocalizing
};

/// \brief Type identifier for Area Target observers
VU_CONST_INT(VU_OBSERVER_AREA_TARGET_TYPE, 0x7);

/// \brief Type identifier for Area Target observations
VU_CONST_INT(VU_OBSERVATION_AREA_TARGET_TYPE, 0x7);

/// \brief Create an Area Target observer from database
VU_API VuResult VU_API_CALL vuEngineCreateAreaTargetObserver(VuEngine* engine, VuObserver** observer, const VuAreaTargetConfig* config, VuAreaTargetCreationError* errorCode);

/// \brief Get all Area Target observers
VU_API VuResult VU_API_CALL vuEngineGetAreaTargetObservers(const VuEngine* engine, VuObserverList* observerList);

/// \brief Get the unique ID associated to the target from an Area Target observer
VU_API VuResult VU_API_CALL vuAreaTargetObserverGetTargetUniqueId(const VuObserver* observer, const char** targetId);

/// \brief Get the name associated to the target from an Area Target observer
VU_API VuResult VU_API_CALL vuAreaTargetObserverGetTargetName(const VuObserver* observer, const char** targetName);

/// \brief Get the size in meters associated to the target from an Area Target observer
VU_API VuResult VU_API_CALL vuAreaTargetObserverGetTargetSize(const VuObserver* observer, VuVector3F* size);

/// \brief Get the pose transformation offset associated to the target from an Area Target observer
VU_API VuResult VU_API_CALL vuAreaTargetObserverGetTargetPoseOffset(const VuObserver* observer, VuMatrix44F* offset);

/// \brief Get the pose transformation offset associated to the target from an Area Target observer
VU_API VuResult VU_API_CALL vuAreaTargetObserverSetTargetPoseOffset(VuObserver* observer, const VuMatrix44F* poseOffset);

/// \brief Get the axis-aligned bounding box associated to the target from an Area Target observer, relative to the target's frame of reference
VU_API VuResult VU_API_CALL vuAreaTargetObserverGetAABB(const VuObserver* observer, VuAABB* bbox);

/// \brief Set approximate position within an Area Target target from an external localization source
/**
 * Setting up the external position only succeeds when the target is activated.
 *
 * \note The provided external position is valid until a successful localization,
 * target deactivation, or updating the external position by a repeated call
 * of this method (whichever happens first).
 *
 * \param observer Observer handle
 * \param position Position (x, z) in target coordinate system (which is y-up).
 * \param radius Uncertainty of the position in meters. Must be positive.
 * \returns VU_SUCCESS on success, VU_FAILED on failure.
 */
VU_API VuResult VU_API_CALL vuAreaTargetObserverSetExternalPosition(VuObserver* observer, VuVector2F position, float radius);

/// \brief Check whether the target requires external positions for initial detection
/**
 * Large targets require external positions to provide an initial hint about
 * which data to load. Until an external position is provided, the large
 * target will not be detected. This is also necessary after an extended loss
 * of tracking, e.g. after pausing the app and moving to a new location.
 *
 * \note The external position should be set using vuAreaTargetObserverSetExternalPosition().
 *
 * \param observer Observer handle
 * \param requiresExternalPositions Gives VU_TRUE if the target needs external positions
 * for initial detection or after extended loss of tracking, VU_FALSE otherwise.
 * \returns VU_SUCCESS on success, VU_FAILED on failure.
 */
VU_API VuResult VU_API_CALL vuAreaTargetObserverRequiresExternalPositions(const VuObserver* observer, VuBool* requiresExternalPositions);

/// \brief Get all Area Target observations
VU_API VuResult VU_API_CALL vuStateGetAreaTargetObservations(const VuState* state, VuObservationList* observationList);

/// \brief Get status info associated with an Area Target observation
VU_API VuResult VU_API_CALL vuAreaTargetObservationGetStatusInfo(const VuObservation* observation, VuAreaTargetObservationStatusInfo* statusInfo);

/// \brief Get target info associated with an Area Target observation
VU_API VuResult VU_API_CALL vuAreaTargetObservationGetTargetInfo(const VuObservation* observation, VuAreaTargetObservationTargetInfo* targetInfo);

/** \} */

#ifdef __cplusplus
}
#endif

#endif // _VU_AREATARGETOBSERVER_H_
