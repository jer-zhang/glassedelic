package hacktx.glasses;

import android.bluetooth.BluetoothDevice;

import me.aflak.bluetooth.Bluetooth;

/**
 * Created by Jerry Zhang on 10/29/2017.
 */

public class CommunicationInterface implements Bluetooth.CommunicationCallback {

    @Override
    public void onConnect(BluetoothDevice device) {

    }

    @Override
    public void onDisconnect(BluetoothDevice device, String message) {

    }

    @Override
    public void onMessage(String message) {

    }

    @Override
    public void onError(String message) {

    }

    @Override
    public void onConnectError(BluetoothDevice device, String message) {

    }
}
