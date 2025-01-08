"use client";
import { useEffect, useState } from "react";
import { Card } from "./Card";
import { Icons } from "./icons";

import {
  Navigation,
  Pagination,
  Scrollbar,
  A11y,
  Autoplay,
  Parallax,
} from "swiper/modules";
import { Swiper, SwiperSlide } from "swiper/react";
import { createClient } from "@supabase/supabase-js";

import "swiper/css";
import "swiper/css/navigation";
import "swiper/css/pagination";

const supabaseUrl = process.env.NEXT_PUBLIC_SUPABASE_URL || "";
const supabaseKey = process.env.NEXT_PUBLIC_SUPABASE_ANON_KEY || "";
const supabase = createClient(supabaseUrl, supabaseKey);

export function Footer() {
  const [tempOut, setTempOut] = useState(0);
  const [humOut, setHumOut] = useState(0);
  const [pressure, setPressure] = useState(0);
  const [rainAnalog, setRainAnalog] = useState(0); // Analog sensor value for rainfall
  const [rainDigital, setRainDigital] = useState(false); // Digital sensor for raining status
  const [airQuality, setAirQuality] = useState(0);
  const [windDirection, setWindDirection] = useState("NA");
  const [altitude, setAltitude] = useState(0);
  const [ldrStatus, setLdrStatus] = useState(false); // Day (true) or Night (false)

  useEffect(() => {
    const fetchWeatherData = async () => {
      try {
        const { data, error } = await supabase
          .from("weather_data")
          .select("*")
          .order("created_at", { ascending: false })
          .limit(1); // Fetch the latest data

        if (error) {
          console.error("Error fetching weather data:", error);
          return;
        }

        if (data && data.length > 0) {
          const latestData = data[0];
          setTempOut(latestData.tempOut);
          setHumOut(latestData.humOut);
          setPressure(latestData.pressure);
          setRainAnalog(latestData.rainAnalog);
          setRainDigital(latestData.rainDigital);
          setAirQuality(latestData.airQuality);
          setWindDirection(latestData.windDirection);
          setAltitude(latestData.altitude);
          setLdrStatus(latestData.ldrStatus);
        }
      } catch (error) {
        console.error("Unexpected error:", error);
      }
    };

    fetchWeatherData();
  }, []);

  return (
    <div className="flex flex-col">
      <div className="flex justify-between mx-4">
        <div className="flex flex-col">
          <div className="flex gap-3">
            <h3 className="text-2xl">Wind</h3>
            <Icons.wind className="h-8 w-8 rounded-full bg-[#0e1426] p-1" />
          </div>
          <div className="flex gap-2">
            <p className="text-xl text-[#5f6281]">Direction:</p>
            <p className="text-xl text-white">{windDirection}</p>
          </div>
          {/* Day/Night Status */}
          <div className="flex gap-3 mt-4 items-center">
            {ldrStatus ? (
              <>
                <Icons.sun className="h-8 w-8 text-yellow-400" />
                <p className="text-xl text-white">Daytime</p>
              </>
            ) : (
              <>
                <Icons.moon className="h-8 w-8 text-blue-400" />
                <p className="text-xl text-white">Nighttime</p>
              </>
            )}
          </div>
          {/* Raining/Not Raining Status */}
          <div className="flex gap-3 mt-4 items-center">
            {rainDigital ? (
              <>
                <Icons.cloudRain className="h-8 w-8 text-blue-500" />
                <p className="text-xl text-white">Raining</p>
              </>
            ) : (
              <>
                <Icons.cloud className="h-8 w-8 text-gray-400" />
                <p className="text-xl text-white"></p>
              </>
            )}
          </div>
        </div>
      </div>
      <div className="flex m-4 gap-3 min-h-60">
        <Swiper
          modules={[Navigation, Pagination, Scrollbar, A11y, Autoplay, Parallax]}
          spaceBetween={10}
          slidesPerView={4}
          navigation
          pagination={{ clickable: true }}
          autoplay
          breakpoints={{
            200: { slidesPerView: 1, spaceBetween: 10 },
            640: { slidesPerView: 2, spaceBetween: 20 },
            768: { slidesPerView: 3, spaceBetween: 30 },
            1024: { slidesPerView: 4, spaceBetween: 10 },
          }}
        >
          <SwiperSlide>
            <Card
              title="Temperature OUT"
              icon={<Icons.temperature />}
              value={tempOut}
              unit={"°F"}
              texts={["", "Outdoor Temperature", ""]} 
            />
          </SwiperSlide>
          <SwiperSlide>
            <Card
              title="Humidity OUT"
              icon={<Icons.droplet />}
              value={humOut}
              unit={"%"}
              texts={["", "Outdoor Humidity", ""]}
            />
          </SwiperSlide>
          <SwiperSlide>
            <Card
              title="Pressure"
              icon={<Icons.gauge />}
              value={pressure}
              unit={"hPa"}
              texts={["", "Absolute Pressure", ""]}
            />
          </SwiperSlide>
          <SwiperSlide>
            <Card
              title="Rainfall"
              icon={<Icons.cloudRain />}
              value={rainAnalog}
              unit={"units"}
              texts={["", "Rain Sensor Value", ""]}
            />
          </SwiperSlide>
          <SwiperSlide>
            <Card
              title="Air Quality"
              icon={<Icons.airQuality />}
              value={airQuality}
              unit={""}
              texts={["", "Air Pollutants", ""]}
            />
          </SwiperSlide>
          <SwiperSlide>
            <Card
              title="Altitude"
              icon={<Icons.altitude />}
              value={altitude}
              unit={"m"}
              texts={["", "Altitude", ""]}
            />
          </SwiperSlide>
        </Swiper>
      </div>
    </div>
  );
}
