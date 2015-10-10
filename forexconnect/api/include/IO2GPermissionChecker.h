#pragma once

/** Permission checker.*/
class IO2GPermissionChecker : public IAddRef
{
 protected:
    IO2GPermissionChecker(){};
 public:

    /** Whether the open market order can be created. */
    virtual O2GPermissionStatus canCreateMarketOpenOrder(const char* instrument) = 0;

    /** Whether the open market order can be changed. */
    virtual O2GPermissionStatus canChangeMarketOpenOrder(const char* instrument) = 0;

    /** Whether the open market order can be deleted. */
    virtual O2GPermissionStatus canDeleteMarketOpenOrder(const char* instrument) = 0;

    /** Whether the close market order can be created. */
    virtual O2GPermissionStatus canCreateMarketCloseOrder(const char* instrument) = 0;

    /** Whether the close market order can be changed. */
    virtual O2GPermissionStatus canChangeMarketCloseOrder(const char* instrument) = 0;

    /** Whether the close market order can be deleted. */
    virtual O2GPermissionStatus canDeleteMarketCloseOrder(const char* instrument) = 0;

    /** Whether the open entry order can be created. */
    virtual O2GPermissionStatus canCreateEntryOrder(const char* instrument) = 0;

    /** Whether the open entry order can be changed. */
    virtual O2GPermissionStatus canChangeEntryOrder(const char* instrument) = 0;

    /** Whether the open entry order can be deleted. */
    virtual O2GPermissionStatus canDeleteEntryOrder(const char* instrument) = 0;

    /** Whether the open entry order can be created. */
    virtual O2GPermissionStatus canCreateStopLimitOrder(const char* instrument) = 0;

    /** Whether the open entry order can be changed. */
    virtual O2GPermissionStatus canChangeStopLimitOrder(const char* instrument) = 0;

    /** Whether the open entry order can be deleted. */
    virtual O2GPermissionStatus canDeleteStopLimitOrder(const char* instrument) = 0;

    /** Whether it is possible to request a quote. */
    virtual O2GPermissionStatus canRequestQuote(const char* instrument) = 0;

    /** Whether it is possible to accept the quote provided by the dealer. */
    virtual O2GPermissionStatus canAcceptQuote(const char* instrument) = 0;

    /** Whether it is possible to delete the quote provided by the dealer. */
    virtual O2GPermissionStatus canDeleteQuote(const char* instrument) = 0;

    /** Whether it is possible to create the new OCO. */
    virtual O2GPermissionStatus canCreateOCO(const char* instrument) = 0;

    /** Whether it is possible to create the new OTO. */
    virtual O2GPermissionStatus canCreateOTO(const char* instrument) = 0;

    /** Whether it is possible to add new orders to the existing ContingencyGroup or creating new. */
    virtual O2GPermissionStatus canJoinToNewContingencyGroup(const char* instrument) = 0;

    /** Whether it is possible to add new orders to the existing ContingencyGroup or creating new. */
    virtual O2GPermissionStatus canJoinToExistingContingencyGroup(const char* instrument) = 0;

    /** Whether it is possible to exclude orders from the existing ContingencyGroup. */
    virtual O2GPermissionStatus canRemoveFromContingencyGroup(const char* instrument) = 0;

    /** Whether it is possible to change the offers subscription. */
    virtual O2GPermissionStatus canChangeOfferSubscription(const char* instrument) = 0;

    /** Determines whether the user may create order for close all positions for an instrument on an account. */
    virtual O2GPermissionStatus canCreateNetCloseOrder(const char* instrument) = 0;

    /** Determines whether the user may change order for close all positions for an instrument on an account. */
    virtual O2GPermissionStatus canChangeNetCloseOrder(const char* instrument) = 0;

    /** Determines whether the user may delete order for close all positions for an instrument on an account. */
    virtual O2GPermissionStatus canDeleteNetCloseOrder(const char* instrument) = 0;

    /** Determines whether the user may create net stop/limit order. */
    virtual O2GPermissionStatus canCreateNetStopLimitOrder(const char* instrument) = 0;

    /** Determines whether the user may change net stop/limit order. */
    virtual O2GPermissionStatus canChangeNetStopLimitOrder(const char* instrument) = 0;

    /** Determines whether the user may delete net stop/limit order. */
    virtual O2GPermissionStatus canDeleteNetStopLimitOrder(const char* instrument) = 0;

    /** Determines whether the trailing stop is dymanic */
    virtual O2GPermissionStatus canUseDynamicTrailingForStop() = 0;

    /** Determines whether the trailing limit is dymanic */
    virtual O2GPermissionStatus canUseDynamicTrailingForLimit() = 0;

    /** Determines whether the trailing entry stop is dymanic */
    virtual O2GPermissionStatus canUseDynamicTrailingForEntryStop() = 0;

    /** Determines whether the trailing entry limit is dymanic */
    virtual O2GPermissionStatus canUseDynamicTrailingForEntryLimit() = 0;

    /** Determines whether the trailing stop is fluctuate */
    virtual O2GPermissionStatus canUseFluctuateTrailingForStop() = 0;

    /** Determines whether the trailing limit is fluctuate */
    virtual O2GPermissionStatus canUseFluctuateTrailingForLimit() = 0;

    /** Determines whether the trailing entry stop is fluctuate */
    virtual O2GPermissionStatus canUseFluctuateTrailingForEntryStop() = 0;

    /** Determines whether the trailing entry limit is fluctuate */
    virtual O2GPermissionStatus canUseFluctuateTrailingForEntryLimit() = 0;
};

