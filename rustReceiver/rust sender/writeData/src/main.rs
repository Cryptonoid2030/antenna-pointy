use serialport::{SerialPort};
use std::time::Duration;
use std::io::Write;

fn main() {
    let port_name = "/dev/ttyACM0";
    let baud_rate = 19200;
    let timeout = Duration::from_millis(1000);

    let mut port = serialport::new(port_name, baud_rate)
    .timeout(timeout)
    .open()
    .expect("Failed to open serial port");

    loop {
        let data = b"Longitude: 42";
        println!("Writing to serial");
        port.write_all(data)
        .expect("Failed to write to serial port");

        std::thread::sleep(Duration::from_millis(2000));
    }


}
