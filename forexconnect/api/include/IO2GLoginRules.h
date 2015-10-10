#pragma once

/** Provide information about loaded table.*/
class Order2Go2 IO2GLoginRules : public IAddRef
{
 protected:
    IO2GLoginRules();
 public:
    /** Check loading table during login.*/
    virtual bool isTableLoadedByDefault(O2GTable table) = 0;
    /** Get response for loaded table.*/
    virtual IO2GResponse* getTableRefreshResponse(O2GTable table) = 0;
    /** Gets system properties.*/
    virtual IO2GResponse* getSystemPropertiesResponse() = 0;
    /** Gets permission checker. */
    virtual IO2GPermissionChecker* getPermissionChecker() = 0;
    /** Gets trading settings provider. */
    virtual IO2GTradingSettingsProvider* getTradingSettingsProvider() = 0;
};


