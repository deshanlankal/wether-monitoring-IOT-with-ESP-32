"use client";
import { useEffect, useState } from "react";
import { createClient } from "@supabase/supabase-js";
import { Icons } from "./icons";

const supabaseUrl = process.env.NEXT_PUBLIC_SUPABASE_URL || "";
const supabaseKey = process.env.NEXT_PUBLIC_SUPABASE_ANON_KEY || "";
const supabase = createClient(supabaseUrl, supabaseKey);

export function Header() {
  const [location, setLocation] = useState("");

  useEffect(() => {
    const fetchLocation = async () => {
      const { data, error } = await supabase
        .from("weather_location")  // Changed to weather_location table
        .select("city, region, country")
        .order("created_at", { ascending: false })
        .limit(1); // Fetching the latest location data

      if (error) {
        console.error("Error fetching location:", error);
        return;
      }

      if (data && data.length > 0) {
        const { city, region, country } = data[0];
        setLocation(`${city}, ${region}, ${country}`);
      }
    };

    fetchLocation();
  }, []);

  return (
    <div className="flex flex-col justify-center md:items-start md:flex-row md:justify-between md:mt-3 md:mx-3">
      <div className="flex flex-col md:mt-0 text-center md:text-left">
        <h1 className="text-xl md:text-4xl font-bold text-shadow-lg shadow-slate-800 uppercase">
          Weather Station
        </h1>
        <div className="flex justify-center md:justify-start items-center gap-1">
          <Icons.currentLocation className="w-6 h-6 md:block" />
          <p className="text-sm md:text-xl text-shadow shadow-slate-800">
            {location || "Loading location..."}
          </p>
        </div>
      </div>
    </div>
  );
}
