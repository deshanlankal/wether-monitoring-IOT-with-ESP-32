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
import { loginTb } from "@/actions";
import { readSetting } from "@/actions";

const supabaseUrl = process.env.NEXT_PUBLIC_SUPABASE_URL || "";
const supabaseKey = process.env.NEXT_PUBLIC_SUPABASE_ANON_KEY || "";
const supabase = createClient(supabaseUrl, supabaseKey);

export function Footer() {
  const [tempIn, setTempIn] = useState(0);
  const [tempOut, setTempOut] = useState(0);
  const [humIn, setHumIn] = useState(0);
  const [humOut, setHumOut] = useState(0);
  const [pressure, setPressure] = useState(0);
  const [rainfall, setRainfall] = useState(0);
  const [uvIndex, setUvIndex] = useState(0);
  const [airQuality, setAirQuality] = useState(0);

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
          setTempIn(latestData.tempIn);
          setTempOut(latestData.tempOut);
          setHumIn(latestData.humIn);
          setHumOut(latestData.humOut);
          setPressure(latestData.pressure);
          setRainfall(latestData.rainfall);
          setUvIndex(latestData.uvIndex);
          setAirQuality(latestData.airQuality);
        }
      } catch (error) {
        console.error("Unexpected error:", error);
      }
    };

    fetchWeatherData();
  }, []);

  return (
    <div className="flex flex-col">
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
              title="Temperature IN"
              icon={<Icons.temperature />}
              value={tempIn}
              unit={"°F"}
              texts={["", "Indoor Temperature", ""]}
            />
          </SwiperSlide>
          <SwiperSlide>
            <Card
              title="Humidity IN"
              icon={<Icons.droplet />}
              value={humIn}
              unit={"%"}
              texts={["", "Indoor Humidity", ""]}
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
              value={rainfall}
              unit={"mm"}
              texts={["", "Total", ""]}
            />
          </SwiperSlide>
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
              title="UV Index"
              icon={<Icons.sun />}
              value={uvIndex}
              unit={""}
              texts={["", "UV Radiation", ""]}
            />
          </SwiperSlide>
          <SwiperSlide>
            <Card
              title="Air Quality"
              icon={<Icons.droplet />}
              value={airQuality}
              unit={""}
              texts={["", "Air Pollutants", ""]}
            />
          </SwiperSlide>
        </Swiper>
      </div>
    </div>
  );
}
