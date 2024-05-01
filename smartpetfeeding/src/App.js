import React from 'react';
import ReactDOM from 'react-dom';
import Navbar from "./navbar.js"
import Status from "./pages/Status.js"
import Home from "./pages/Home"
import About from "./pages/About"
import { Route, Routes } from "react-router-dom"


function App() {
  return (
    <>
      <Navbar />
      <div className="container">
        <Routes>
          <Route path="/" element={<Home />} />
          <Route path="/about" element={<About />} />
          <Route path="/status" element={<Status />} />
        </Routes>
      </div>
    </>
  )
}

export default App